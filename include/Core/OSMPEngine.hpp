#ifndef _OSMPENGINE_HPP_
#define _OSMPENGINE_HPP_

#include <vector>
#include <string>
#include <memory>
#include <map>

namespace OpenSampler {

struct Sample {
    std::vector<float> data;
    int sampleRate;
    int channels;
    int rootNote;
    int loVel;
    int hiVel;
    int loKey;
    int hiKey;
    bool looping;
    int loopStart;
    int loopEnd;
    
    Sample() : sampleRate(44100), channels(1), rootNote(60), 
               loVel(0), hiVel(127), loKey(0), hiKey(127),
               looping(false), loopStart(0), loopEnd(0) {}
};

struct Voice {
    int note;
    int velocity;
    float phase;
    float gain;
    bool active;
    const Sample* sample;
    
    Voice() : note(-1), velocity(0), phase(0.0f), gain(0.0f), 
              active(false), sample(nullptr) {}
};

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
    
    int GetNumVoices() const { return static_cast<int>(mVoices.size()); }
    int GetActiveVoices() const;
    int GetNumSamples() const { return static_cast<int>(mSamples.size()); }
    
private:
    Voice* AllocateVoice(int note, int velocity);
    const Sample* FindSample(int note, int velocity);
    void ProcessVoice(Voice& voice, float** outputs, int nFrames, int nChans);
    
    double mSampleRate;
    int mBlockSize;
    float mMasterVolume;
    
    float mAttack;
    float mDecay;
    float mSustain;
    float mRelease;
    
    std::vector<Voice> mVoices;
    std::vector<std::unique_ptr<Sample>> mSamples;
    
    static constexpr int kMaxVoices = 32;
};

} // namespace OpenSampler

#endif
