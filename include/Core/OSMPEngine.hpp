#ifndef _OSMPENGINE_HPP_
#define _OSMPENGINE_HPP_

#include "Core/Audio/Audio.hpp"
#include <memory>
#include <string>

namespace OpenSampler {

class OSMPEngine {
public:
    OSMPEngine();
    ~OSMPEngine();
    
    void Init(double sampleRate, int blockSize);
    void Reset();
    
    void ProcessBlock(float** outputs, int nFrames, int nChans);
    
    void NoteOn(int note, int velocity);
    void NoteOff(int note);
    void AllNotesOff();
    
    bool LoadSample(const std::string& path, int rootNote = 60);
    void ClearSamples();
    
    void SetMasterVolume(float volume);
    float GetMasterVolume() const { return mMasterVolume; }
    
    void SetAttack(float attack);
    void SetDecay(float decay);
    void SetSustain(float sustain);
    void SetRelease(float release);
    
    float GetAttack() const { return mAttack; }
    float GetDecay() const { return mDecay; }
    float GetSustain() const { return mSustain; }
    float GetRelease() const { return mRelease; }
    
    int GetNumVoices() const;
    int GetActiveVoices() const;
    int GetNumSamples() const;
    
    Audio::SampleBank* GetSampleBank() { return mSampleBank.get(); }
    const Audio::SampleBank* GetSampleBank() const { return mSampleBank.get(); }
    
private:
    double mSampleRate;
    int mBlockSize;
    float mMasterVolume;
    
    float mAttack;
    float mDecay;
    float mSustain;
    float mRelease;
    
    std::unique_ptr<Audio::VoiceManager> mVoiceManager;
    std::unique_ptr<Audio::SampleBank> mSampleBank;
    
    static constexpr int kMaxVoices = 32;
};

} // namespace OpenSampler

#endif
