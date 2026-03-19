#include "Core/Audio/Audio.hpp"
#include <algorithm>
#include <cmath>

namespace OpenSampler {
namespace Audio {

float ADSREnvelope::Process(float sampleRate) {
    const float dt = 1.0f / sampleRate;
    
    switch (state) {
        case VoiceState::Idle:
            currentLevel = 0.0f;
            break;
            
        case VoiceState::Attack:
            if (attackTime > 0.0f) {
                currentLevel += dt / attackTime;
                if (currentLevel >= 1.0f) {
                    currentLevel = 1.0f;
                    state = VoiceState::Decay;
                }
            } else {
                currentLevel = 1.0f;
                state = VoiceState::Decay;
            }
            break;
            
        case VoiceState::Decay:
            if (decayTime > 0.0f) {
                float target = sustainLevel;
                float delta = (target - currentLevel) * dt / decayTime;
                currentLevel += delta;
                
                if (std::abs(currentLevel - target) < 0.001f) {
                    currentLevel = target;
                    state = VoiceState::Sustain;
                }
            } else {
                currentLevel = sustainLevel;
                state = VoiceState::Sustain;
            }
            break;
            
        case VoiceState::Sustain:
            currentLevel = sustainLevel;
            break;
            
        case VoiceState::Release:
            if (releaseTime > 0.0f) {
                currentLevel -= dt / releaseTime;
                if (currentLevel <= 0.0f) {
                    currentLevel = 0.0f;
                    state = VoiceState::Finished;
                }
            } else {
                currentLevel = 0.0f;
                state = VoiceState::Finished;
            }
            break;
            
        case VoiceState::Finished:
            currentLevel = 0.0f;
            break;
    }
    
    return currentLevel;
}

Voice::Voice()
    : mActive(false)
    , mNote(-1)
    , mVelocity(0)
    , mSampleRate(44100.0)
    , mPlayhead(0.0)
    , mPlaybackSpeed(1.0)
    , mAge(0.0f)
    , mRegion(nullptr)
    , mVelocityGain(1.0f)
    , mPanLeft(1.0f)
    , mPanRight(1.0f)
    , mLoopingForward(true)
{
}

Voice::~Voice() {
}

void Voice::Init(double sampleRate) {
    mSampleRate = sampleRate;
    Reset();
}

void Voice::Reset() {
    mActive = false;
    mNote = -1;
    mVelocity = 0;
    mPlayhead = 0.0;
    mPlaybackSpeed = 1.0;
    mAge = 0.0f;
    mRegion = nullptr;
    mEnvelope.Reset();
    mVelocityGain = 1.0f;
    mPanLeft = 1.0f;
    mPanRight = 1.0f;
    mLoopingForward = true;
}

void Voice::NoteOn(int note, int velocity, const SampleRegion* region) {
    if (!region || !region->sampleData || !region->sampleData->IsValid()) {
        return;
    }
    
    mNote = note;
    mVelocity = velocity;
    mRegion = region;
    mActive = true;
    mPlayhead = 0.0;
    mAge = 0.0f;
    mLoopingForward = true;
    
    mVelocityGain = velocity / 127.0f;
    
    float pan = region->pan;
    if (pan <= 0.0f) {
        mPanLeft = 1.0f;
        mPanRight = 1.0f + pan;
    } else {
        mPanLeft = 1.0f - pan;
        mPanRight = 1.0f;
    }
    
    mPlaybackSpeed = CalculatePitchRatio();
    
    mEnvelope.Reset();
    mEnvelope.TriggerAttack();
}

void Voice::NoteOff() {
    mEnvelope.TriggerRelease();
}

void Voice::Process(float* leftOut, float* rightOut, int numFrames) {
    if (!mActive || !mRegion || !mRegion->sampleData) {
        return;
    }
    
    const SampleData* sampleData = mRegion->sampleData.get();
    if (!sampleData->IsValid()) {
        mActive = false;
        return;
    }
    
    for (int i = 0; i < numFrames; i++) {
        float envLevel = mEnvelope.Process(static_cast<float>(mSampleRate));
        
        if (mEnvelope.state == VoiceState::Finished) {
            mActive = false;
            break;
        }
        
        float sampleLeft = GetSample(0);
        float sampleRight = sampleData->numChannels > 1 ? GetSample(1) : sampleLeft;
        
        float gain = mVelocityGain * mRegion->volume * envLevel;
        
        leftOut[i] += sampleLeft * gain * mPanLeft;
        rightOut[i] += sampleRight * gain * mPanRight;
        
        AdvancePlayhead();
        mAge += 1.0f / static_cast<float>(mSampleRate);
    }
}

void Voice::SetEnvelope(float attack, float decay, float sustain, float release) {
    mEnvelope.attackTime = attack;
    mEnvelope.decayTime = decay;
    mEnvelope.sustainLevel = sustain;
    mEnvelope.releaseTime = release;
}

float Voice::GetSample(int channel) {
    if (!mRegion || !mRegion->sampleData) {
        return 0.0f;
    }
    
    const SampleData* sampleData = mRegion->sampleData.get();
    if (!sampleData->IsValid()) {
        return 0.0f;
    }
    
    int index = static_cast<int>(mPlayhead);
    if (index < 0 || index >= sampleData->numFrames) {
        return 0.0f;
    }
    
    int actualChannel = std::min(channel, sampleData->numChannels - 1);
    int sampleIndex = index * sampleData->numChannels + actualChannel;
    
    if (sampleIndex >= 0 && sampleIndex < static_cast<int>(sampleData->audioData.size())) {
        return sampleData->audioData[sampleIndex];
    }
    
    return 0.0f;
}

void Voice::AdvancePlayhead() {
    if (!mRegion || !mRegion->sampleData) {
        mActive = false;
        return;
    }
    
    const SampleData* sampleData = mRegion->sampleData.get();
    double sampleRateRatio = static_cast<double>(sampleData->sampleRate) / mSampleRate;
    double increment = mPlaybackSpeed * sampleRateRatio;
    
    if (mRegion->loopMode == LoopMode::None) {
        mPlayhead += increment;
        if (mPlayhead >= sampleData->numFrames) {
            mActive = false;
        }
    }
    else if (mRegion->loopMode == LoopMode::Forward) {
        mPlayhead += increment;
        
        if (mRegion->loopEnd > mRegion->loopStart && mRegion->loopEnd <= sampleData->numFrames) {
            if (mPlayhead >= mRegion->loopEnd) {
                mPlayhead = mRegion->loopStart + (mPlayhead - mRegion->loopEnd);
            }
        } else if (mPlayhead >= sampleData->numFrames) {
            mPlayhead = 0.0;
        }
    }
    else if (mRegion->loopMode == LoopMode::PingPong) {
        if (mLoopingForward) {
            mPlayhead += increment;
            if (mRegion->loopEnd > mRegion->loopStart && mPlayhead >= mRegion->loopEnd) {
                mLoopingForward = false;
                mPlayhead = mRegion->loopEnd - (mPlayhead - mRegion->loopEnd);
            }
        } else {
            mPlayhead -= increment;
            if (mPlayhead <= mRegion->loopStart) {
                mLoopingForward = true;
                mPlayhead = mRegion->loopStart + (mRegion->loopStart - mPlayhead);
            }
        }
    }
    else if (mRegion->loopMode == LoopMode::Backward) {
        mPlayhead -= increment;
        if (mPlayhead < 0.0) {
            if (mRegion->loopEnd > 0) {
                mPlayhead = mRegion->loopEnd + mPlayhead;
            } else {
                mPlayhead = sampleData->numFrames + mPlayhead;
            }
        }
    }
}

float Voice::CalculatePitchRatio() const {
    if (!mRegion) {
        return 1.0f;
    }
    
    float semitones = static_cast<float>(mNote - mRegion->rootNote);
    semitones += mRegion->tune;
    semitones += mRegion->fineTune / 100.0f;
    
    return std::pow(2.0f, semitones / 12.0f);
}

VoiceManager::VoiceManager(int maxVoices)
    : mMaxVoices(maxVoices)
    , mSampleRate(44100.0)
    , mAttack(0.01f)
    , mDecay(0.1f)
    , mSustain(0.7f)
    , mRelease(0.2f)
{
    mVoices.reserve(mMaxVoices);
    for (int i = 0; i < mMaxVoices; i++) {
        mVoices.push_back(std::make_unique<Voice>());
    }
}

VoiceManager::~VoiceManager() {
}

void VoiceManager::Init(double sampleRate) {
    mSampleRate = sampleRate;
    for (auto& voice : mVoices) {
        voice->Init(sampleRate);
    }
}

void VoiceManager::Reset() {
    for (auto& voice : mVoices) {
        voice->Reset();
    }
}

Voice* VoiceManager::AllocateVoice(int note, int velocity) {
    Voice* freeVoice = FindFreeVoice();
    if (freeVoice) {
        return freeVoice;
    }
    
    return StealVoice();
}

void VoiceManager::ReleaseVoice(int note) {
    for (auto& voice : mVoices) {
        if (voice->IsActive() && voice->GetNote() == note) {
            voice->NoteOff();
        }
    }
}

void VoiceManager::ReleaseAllVoices() {
    for (auto& voice : mVoices) {
        if (voice->IsActive()) {
            voice->NoteOff();
        }
    }
}

void VoiceManager::Process(float** outputs, int numFrames, int numChannels) {
    if (numChannels < 2) {
        return;
    }
    
    for (int i = 0; i < numFrames; i++) {
        outputs[0][i] = 0.0f;
        outputs[1][i] = 0.0f;
    }
    
    for (auto& voice : mVoices) {
        if (voice->IsActive()) {
            voice->Process(outputs[0], outputs[1], numFrames);
        }
    }
}

int VoiceManager::GetActiveVoiceCount() const {
    int count = 0;
    for (const auto& voice : mVoices) {
        if (voice->IsActive()) {
            count++;
        }
    }
    return count;
}

void VoiceManager::SetEnvelope(float attack, float decay, float sustain, float release) {
    mAttack = attack;
    mDecay = decay;
    mSustain = sustain;
    mRelease = release;
    
    for (auto& voice : mVoices) {
        voice->SetEnvelope(attack, decay, sustain, release);
    }
}

Voice* VoiceManager::FindFreeVoice() {
    for (auto& voice : mVoices) {
        if (!voice->IsActive()) {
            return voice.get();
        }
    }
    return nullptr;
}

Voice* VoiceManager::StealVoice() {
    Voice* oldest = nullptr;
    float maxAge = -1.0f;
    
    for (auto& voice : mVoices) {
        if (voice->GetAge() > maxAge) {
            maxAge = voice->GetAge();
            oldest = voice.get();
        }
    }
    
    if (oldest) {
        oldest->Reset();
    }
    
    return oldest;
}

SampleBank::SampleBank() {
}

SampleBank::~SampleBank() {
    Clear();
}

void SampleBank::Clear() {
    mRegions.clear();
}

bool SampleBank::LoadSample(const std::string& path, int rootNote) {
    return false;
}

void SampleBank::AddRegion(std::shared_ptr<SampleRegion> region) {
    if (region) {
        mRegions.push_back(region);
    }
}

const SampleRegion* SampleBank::FindRegion(int note, int velocity) const {
    for (const auto& region : mRegions) {
        if (region->MatchesNote(note, velocity)) {
            return region.get();
        }
    }
    return nullptr;
}

const SampleRegion* SampleBank::GetRegion(int index) const {
    if (index >= 0 && index < static_cast<int>(mRegions.size())) {
        return mRegions[index].get();
    }
    return nullptr;
}

} // namespace Audio
} // namespace OpenSampler
