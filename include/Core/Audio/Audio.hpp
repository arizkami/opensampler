#ifndef _OSMP_AUDIO_HPP_
#define _OSMP_AUDIO_HPP_

#include <vector>
#include <memory>
#include <string>
#include <atomic>
#include <cstdint>

namespace OpenSampler {
namespace Audio {

enum class VoiceState {
    Idle,
    Attack,
    Decay,
    Sustain,
    Release,
    Finished
};

enum class LoopMode {
    None,
    Forward,
    PingPong,
    Backward
};

struct SampleData {
    std::vector<float> audioData;
    int sampleRate;
    int numChannels;
    int numFrames;
    
    SampleData() 
        : sampleRate(44100)
        , numChannels(1)
        , numFrames(0) 
    {}
    
    void Clear() {
        audioData.clear();
        numFrames = 0;
    }
    
    bool IsValid() const {
        return !audioData.empty() && numFrames > 0;
    }
};

struct SampleRegion {
    int rootNote;
    int lowKey;
    int highKey;
    int lowVelocity;
    int highVelocity;
    
    int loopStart;
    int loopEnd;
    LoopMode loopMode;
    
    float tune;
    float fineTune;
    float volume;
    float pan;
    
    std::shared_ptr<SampleData> sampleData;
    
    SampleRegion()
        : rootNote(60)
        , lowKey(0)
        , highKey(127)
        , lowVelocity(0)
        , highVelocity(127)
        , loopStart(0)
        , loopEnd(0)
        , loopMode(LoopMode::None)
        , tune(0.0f)
        , fineTune(0.0f)
        , volume(1.0f)
        , pan(0.0f)
        , sampleData(nullptr)
    {}
    
    bool MatchesNote(int note, int velocity) const {
        return (note >= lowKey && note <= highKey &&
                velocity >= lowVelocity && velocity <= highVelocity);
    }
};

struct ADSREnvelope {
    float attackTime;
    float decayTime;
    float sustainLevel;
    float releaseTime;
    
    float currentLevel;
    VoiceState state;
    
    ADSREnvelope()
        : attackTime(0.01f)
        , decayTime(0.1f)
        , sustainLevel(0.7f)
        , releaseTime(0.2f)
        , currentLevel(0.0f)
        , state(VoiceState::Idle)
    {}
    
    void Reset() {
        currentLevel = 0.0f;
        state = VoiceState::Idle;
    }
    
    void TriggerAttack() {
        state = VoiceState::Attack;
    }
    
    void TriggerRelease() {
        state = VoiceState::Release;
    }
    
    float Process(float sampleRate);
    bool IsActive() const {
        return state != VoiceState::Idle && state != VoiceState::Finished;
    }
};

class Voice {
public:
    Voice();
    ~Voice();
    
    void Init(double sampleRate);
    void Reset();
    
    void NoteOn(int note, int velocity, const SampleRegion* region);
    void NoteOff();
    
    void Process(float* leftOut, float* rightOut, int numFrames);
    
    bool IsActive() const { return mActive; }
    int GetNote() const { return mNote; }
    int GetVelocity() const { return mVelocity; }
    float GetAge() const { return mAge; }
    
    void SetEnvelope(float attack, float decay, float sustain, float release);
    
private:
    float GetSample(int channel);
    void AdvancePlayhead();
    float CalculatePitchRatio() const;
    
    bool mActive;
    int mNote;
    int mVelocity;
    
    double mSampleRate;
    double mPlayhead;
    double mPlaybackSpeed;
    float mAge;
    
    const SampleRegion* mRegion;
    ADSREnvelope mEnvelope;
    
    float mVelocityGain;
    float mPanLeft;
    float mPanRight;
    
    bool mLoopingForward;
};

class VoiceManager {
public:
    VoiceManager(int maxVoices = 32);
    ~VoiceManager();
    
    void Init(double sampleRate);
    void Reset();
    
    Voice* AllocateVoice(int note, int velocity);
    void ReleaseVoice(int note);
    void ReleaseAllVoices();
    
    void Process(float** outputs, int numFrames, int numChannels);
    
    int GetActiveVoiceCount() const;
    int GetMaxVoices() const { return mMaxVoices; }
    
    void SetEnvelope(float attack, float decay, float sustain, float release);
    
private:
    Voice* FindFreeVoice();
    Voice* StealVoice();
    
    std::vector<std::unique_ptr<Voice>> mVoices;
    int mMaxVoices;
    double mSampleRate;
    
    float mAttack;
    float mDecay;
    float mSustain;
    float mRelease;
};

class SampleBank {
public:
    SampleBank();
    ~SampleBank();
    
    void Clear();
    
    bool LoadSample(const std::string& path, int rootNote = 60);
    void AddRegion(std::shared_ptr<SampleRegion> region);
    
    const SampleRegion* FindRegion(int note, int velocity) const;
    
    int GetRegionCount() const { return static_cast<int>(mRegions.size()); }
    const SampleRegion* GetRegion(int index) const;
    
private:
    std::vector<std::shared_ptr<SampleRegion>> mRegions;
};

} // namespace Audio
} // namespace OpenSampler

#endif
