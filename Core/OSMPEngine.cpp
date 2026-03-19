#include "Core/OSMPEngine.hpp"
#include <algorithm>
#include <cmath>

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
    mVoices.resize(kMaxVoices);
}

OSMPEngine::~OSMPEngine() {
    ClearSamples();
}

void OSMPEngine::Init(double sampleRate, int blockSize) {
    mSampleRate = sampleRate;
    mBlockSize = blockSize;
    Reset();
}

void OSMPEngine::Reset() {
    AllNotesOff();
    for (auto& voice : mVoices) {
        voice = Voice();
    }
}

void OSMPEngine::ProcessBlock(float** outputs, int nFrames, int nChans) {
    for (int i = 0; i < nFrames; i++) {
        for (int c = 0; c < nChans; c++) {
            outputs[c][i] = 0.0f;
        }
    }
    
    for (auto& voice : mVoices) {
        if (voice.active) {
            ProcessVoice(voice, outputs, nFrames, nChans);
        }
    }
}

void OSMPEngine::NoteOn(int note, int velocity) {
    Voice* voice = AllocateVoice(note, velocity);
    if (voice) {
        voice->note = note;
        voice->velocity = velocity;
        voice->phase = 0.0f;
        voice->gain = velocity / 127.0f;
        voice->active = true;
        voice->sample = FindSample(note, velocity);
    }
}

void OSMPEngine::NoteOff(int note) {
    for (auto& voice : mVoices) {
        if (voice.active && voice.note == note) {
            voice.active = false;
        }
    }
}

void OSMPEngine::AllNotesOff() {
    for (auto& voice : mVoices) {
        voice.active = false;
    }
}

bool OSMPEngine::LoadSample(const std::string& path, int rootNote) {
    return false;
}

void OSMPEngine::ClearSamples() {
    mSamples.clear();
}

void OSMPEngine::SetMasterVolume(float volume) {
    mMasterVolume = std::clamp(volume, 0.0f, 1.0f);
}

void OSMPEngine::SetAttack(float attack) {
    mAttack = std::max(0.001f, attack);
}

void OSMPEngine::SetDecay(float decay) {
    mDecay = std::max(0.001f, decay);
}

void OSMPEngine::SetSustain(float sustain) {
    mSustain = std::clamp(sustain, 0.0f, 1.0f);
}

void OSMPEngine::SetRelease(float release) {
    mRelease = std::max(0.001f, release);
}

int OSMPEngine::GetActiveVoices() const {
    int count = 0;
    for (const auto& voice : mVoices) {
        if (voice.active) count++;
    }
    return count;
}

Voice* OSMPEngine::AllocateVoice(int note, int velocity) {
    for (auto& voice : mVoices) {
        if (!voice.active) {
            return &voice;
        }
    }
    
    Voice* oldest = &mVoices[0];
    for (auto& voice : mVoices) {
        if (voice.phase > oldest->phase) {
            oldest = &voice;
        }
    }
    return oldest;
}

const Sample* OSMPEngine::FindSample(int note, int velocity) {
    for (const auto& sample : mSamples) {
        if (note >= sample->loKey && note <= sample->hiKey &&
            velocity >= sample->loVel && velocity <= sample->hiVel) {
            return sample.get();
        }
    }
    return nullptr;
}

void OSMPEngine::ProcessVoice(Voice& voice, float** outputs, int nFrames, int nChans) {
    if (!voice.sample || voice.sample->data.empty()) {
        voice.active = false;
        return;
    }
    
    const Sample* sample = voice.sample;
    float pitchRatio = std::pow(2.0f, (voice.note - sample->rootNote) / 12.0f);
    float increment = pitchRatio * sample->sampleRate / static_cast<float>(mSampleRate);
    
    for (int i = 0; i < nFrames; i++) {
        int index = static_cast<int>(voice.phase);
        
        if (index >= static_cast<int>(sample->data.size())) {
            if (sample->looping && sample->loopEnd > sample->loopStart) {
                voice.phase = static_cast<float>(sample->loopStart);
                index = sample->loopStart;
            } else {
                voice.active = false;
                break;
            }
        }
        
        float sampleValue = sample->data[index];
        float outputValue = sampleValue * voice.gain * mMasterVolume;
        
        for (int c = 0; c < nChans; c++) {
            outputs[c][i] += outputValue;
        }
        
        voice.phase += increment;
    }
}

} // namespace OpenSampler
