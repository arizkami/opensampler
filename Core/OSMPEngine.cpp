#include "Core/OSMPEngine.hpp"
#include <algorithm>

namespace OpenSampler {

OSMPEngine::OSMPEngine()
    : mSampleRate(44100.0)
    , mBlockSize(512)
    , mMasterVolume(0.8f)
    , mAttack(0.01f)
    , mDecay(0.1f)
    , mSustain(0.7f)
    , mRelease(0.2f)
{
    mVoiceManager = std::make_unique<Audio::VoiceManager>(kMaxVoices);
    mSampleBank = std::make_unique<Audio::SampleBank>();
}

OSMPEngine::~OSMPEngine() {
}

void OSMPEngine::Init(double sampleRate, int blockSize) {
    mSampleRate = sampleRate;
    mBlockSize = blockSize;
    
    if (mVoiceManager) {
        mVoiceManager->Init(sampleRate);
        mVoiceManager->SetEnvelope(mAttack, mDecay, mSustain, mRelease);
    }
    
    Reset();
}

void OSMPEngine::Reset() {
    if (mVoiceManager) {
        mVoiceManager->Reset();
    }
}

void OSMPEngine::ProcessBlock(float** outputs, int nFrames, int nChans) {
    if (!mVoiceManager) {
        for (int i = 0; i < nFrames; i++) {
            for (int c = 0; c < nChans; c++) {
                outputs[c][i] = 0.0f;
            }
        }
        return;
    }
    
    mVoiceManager->Process(outputs, nFrames, nChans);
    
    for (int i = 0; i < nFrames; i++) {
        for (int c = 0; c < nChans; c++) {
            outputs[c][i] *= mMasterVolume;
        }
    }
}

void OSMPEngine::NoteOn(int note, int velocity) {
    if (!mVoiceManager || !mSampleBank) {
        return;
    }
    
    const Audio::SampleRegion* region = mSampleBank->FindRegion(note, velocity);
    if (!region) {
        return;
    }
    
    Audio::Voice* voice = mVoiceManager->AllocateVoice(note, velocity);
    if (voice) {
        voice->SetEnvelope(mAttack, mDecay, mSustain, mRelease);
        voice->NoteOn(note, velocity, region);
    }
}

void OSMPEngine::NoteOff(int note) {
    if (mVoiceManager) {
        mVoiceManager->ReleaseVoice(note);
    }
}

void OSMPEngine::AllNotesOff() {
    if (mVoiceManager) {
        mVoiceManager->ReleaseAllVoices();
    }
}

bool OSMPEngine::LoadSample(const std::string& path, int rootNote) {
    if (!mSampleBank) {
        return false;
    }
    
    return mSampleBank->LoadSample(path, rootNote);
}

void OSMPEngine::ClearSamples() {
    if (mSampleBank) {
        mSampleBank->Clear();
    }
}

void OSMPEngine::SetMasterVolume(float volume) {
    mMasterVolume = std::clamp(volume, 0.0f, 1.0f);
}

void OSMPEngine::SetAttack(float attack) {
    mAttack = std::max(0.001f, attack);
    if (mVoiceManager) {
        mVoiceManager->SetEnvelope(mAttack, mDecay, mSustain, mRelease);
    }
}

void OSMPEngine::SetDecay(float decay) {
    mDecay = std::max(0.001f, decay);
    if (mVoiceManager) {
        mVoiceManager->SetEnvelope(mAttack, mDecay, mSustain, mRelease);
    }
}

void OSMPEngine::SetSustain(float sustain) {
    mSustain = std::clamp(sustain, 0.0f, 1.0f);
    if (mVoiceManager) {
        mVoiceManager->SetEnvelope(mAttack, mDecay, mSustain, mRelease);
    }
}

void OSMPEngine::SetRelease(float release) {
    mRelease = std::max(0.001f, release);
    if (mVoiceManager) {
        mVoiceManager->SetEnvelope(mAttack, mDecay, mSustain, mRelease);
    }
}

int OSMPEngine::GetNumVoices() const {
    return mVoiceManager ? mVoiceManager->GetMaxVoices() : 0;
}

int OSMPEngine::GetActiveVoices() const {
    return mVoiceManager ? mVoiceManager->GetActiveVoiceCount() : 0;
}

int OSMPEngine::GetNumSamples() const {
    return mSampleBank ? mSampleBank->GetRegionCount() : 0;
}

} // namespace OpenSampler
