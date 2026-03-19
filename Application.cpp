#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include "config.h"
#include "Core/OSMPEngine.hpp"
#include "Graphic/OSMPGui.hpp"

using namespace iplug;
using namespace igraphics;

class OpenSampler final : public Plugin
{
public:
  OpenSampler(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets))
  {
    mEngine = std::make_unique<OpenSampler::OSMPEngine>();
    
    GetParam(kParamGain)->InitDouble("Gain", 0.8, 0.0, 1.0, 0.01, "%");
    GetParam(kParamAttack)->InitDouble("Attack", 0.01, 0.001, 2.0, 0.001, "s");
    GetParam(kParamDecay)->InitDouble("Decay", 0.1, 0.001, 2.0, 0.001, "s");
    GetParam(kParamSustain)->InitDouble("Sustain", 0.7, 0.0, 1.0, 0.01, "%");
    GetParam(kParamRelease)->InitDouble("Release", 0.2, 0.001, 5.0, 0.001, "s");

#if IPLUG_EDITOR
    mMakeGraphicsFunc = [&]() {
      return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
    };
    
    mLayoutFunc = [&](IGraphics* pGraphics) {
      pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
      pGraphics->AttachPanelBackground(COLOR_DARK_GRAY);
      pGraphics->LoadFont("Inter-Regular", INTER_REGULAR_FN);
      pGraphics->LoadFont("Inter-Bold", INTER_BOLD_FN);
      pGraphics->LoadFont("Inter-SemiBold", INTER_SEMIBOLD_FN);
      pGraphics->LoadFont("Inter-Medium", INTER_MEDIUM_FN);
      pGraphics->LoadFont("Inter-Light", INTER_LIGHT_FN);
      
      const IRECT b = pGraphics->GetBounds();
      
      mGui = std::make_unique<OpenSampler::OSMPGui>(pGraphics, mEngine.get());
      mGui->CreateControls();
    };
#endif
  }

#if IPLUG_DSP
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override
  {
    const int nChans = NOutChansConnected();
    
    if (mEngine) {
      mEngine->ProcessBlock(outputs, nFrames, nChans);
    }
  }
  
  void OnReset() override
  {
    if (mEngine) {
      mEngine->Init(GetSampleRate(), GetBlockSize());
    }
  }
  
  void ProcessMidiMsg(const IMidiMsg& msg) override
  {
    TRACE;
    
    if (!mEngine) return;
    
    switch (msg.StatusMsg())
    {
      case IMidiMsg::kNoteOn:
        if (msg.Velocity() > 0) {
          mEngine->NoteOn(msg.NoteNumber(), msg.Velocity());
        } else {
          mEngine->NoteOff(msg.NoteNumber());
        }
        break;
        
      case IMidiMsg::kNoteOff:
        mEngine->NoteOff(msg.NoteNumber());
        break;
        
      case IMidiMsg::kControlChange:
        if (msg.ControlChangeIdx() == IMidiMsg::kAllNotesOff) {
          mEngine->AllNotesOff();
        }
        break;
        
      default:
        break;
    }
    
    SendMidiMsg(msg);
  }
  
  void OnParamChange(int paramIdx) override
  {
    if (!mEngine) return;
    
    switch (paramIdx)
    {
      case kParamGain:
        mEngine->SetMasterVolume(GetParam(kParamGain)->Value());
        break;
      case kParamAttack:
        mEngine->SetAttack(GetParam(kParamAttack)->Value());
        break;
      case kParamDecay:
        mEngine->SetDecay(GetParam(kParamDecay)->Value());
        break;
      case kParamSustain:
        mEngine->SetSustain(GetParam(kParamSustain)->Value());
        break;
      case kParamRelease:
        mEngine->SetRelease(GetParam(kParamRelease)->Value());
        break;
      default:
        break;
    }
  }
#endif

private:
  enum EParams
  {
    kParamGain = 0,
    kParamAttack,
    kParamDecay,
    kParamSustain,
    kParamRelease,
    kNumParams
  };
  
  enum EPresets
  {
    kNumPresets = 1
  };
  
  std::unique_ptr<OpenSampler::OSMPEngine> mEngine;
  std::unique_ptr<OpenSampler::OSMPGui> mGui;
};
