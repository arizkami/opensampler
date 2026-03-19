#include "Graphic/OSMPGui.hpp"
#include "Core/OSMPEngine.hpp"
#include "IGraphics.h"
#include "IControls.h"

using namespace iplug;
using namespace igraphics;

namespace OpenSampler {

OSMPGui::OSMPGui(IGraphics* pGraphics, OSMPEngine* pEngine)
    : mGraphics(pGraphics)
    , mEngine(pEngine)
{
}

OSMPGui::~OSMPGui() {
}

void OSMPGui::CreateControls() {
    if (!mGraphics || !mEngine) return;
    
    CreateHeader();
    CreateEnvelopeSection();
    CreateVolumeSection();
    CreateInfoSection();
}

void OSMPGui::OnResize(int width, int height) {
}

void OSMPGui::CreateHeader() {
    const IRECT bounds = mGraphics->GetBounds();
    IRECT headerRect = bounds.GetFromTop(kHeaderHeight).GetPadded(-kPadding);
    
    IText titleText = IText(32, COLOR_WHITE, "Inter-Bold", EAlign::Center, EVAlign::Middle);
    mGraphics->AttachControl(new ITextControl(headerRect, "OpenSampler", titleText));
}

void OSMPGui::CreateEnvelopeSection() {
    const IRECT bounds = mGraphics->GetBounds();
    IRECT envelopeArea = bounds.GetGridCell(0, 1, 2, 3).GetPadded(-kPadding);
    
    IText labelText = IText(14, COLOR_WHITE, "Inter-Regular", EAlign::Center, EVAlign::Top);
    IText sectionText = IText(18, COLOR_WHITE, "Inter-SemiBold", EAlign::Center, EVAlign::Top);
    
    IRECT titleRect = envelopeArea.GetFromTop(30);
    mGraphics->AttachControl(new ITextControl(titleRect, "ENVELOPE", sectionText));
    
    IRECT knobArea = envelopeArea.GetReducedFromTop(40);
    int knobSize = 80;
    int spacing = (knobArea.W() - (4 * knobSize)) / 5;
    
    auto createKnob = [&](int index, const char* label, float min, float max, float defaultVal, 
                          std::function<void(float)> setter) {
        IRECT knobRect = knobArea.GetFromLeft(knobSize).GetMidVPadded(knobSize)
                                  .GetTranslated((knobSize + spacing) * index + spacing, 0);
        
        auto action = [setter](IControl* pCaller) {
            float value = pCaller->GetValue();
            setter(value);
        };
        
        mGraphics->AttachControl(new IVKnobControl(knobRect, action, label, 
                                                   DEFAULT_STYLE, true, min, max, defaultVal));
    };
    
    createKnob(0, "Attack", 0.001f, 2.0f, 0.01f, 
               [this](float v) { mEngine->SetAttack(v); });
    createKnob(1, "Decay", 0.001f, 2.0f, 0.1f, 
               [this](float v) { mEngine->SetDecay(v); });
    createKnob(2, "Sustain", 0.0f, 1.0f, 0.7f, 
               [this](float v) { mEngine->SetSustain(v); });
    createKnob(3, "Release", 0.001f, 5.0f, 0.2f, 
               [this](float v) { mEngine->SetRelease(v); });
}

void OSMPGui::CreateVolumeSection() {
    const IRECT bounds = mGraphics->GetBounds();
    IRECT volumeArea = bounds.GetGridCell(1, 1, 2, 3).GetPadded(-kPadding);
    
    IText sectionText = IText(18, COLOR_WHITE, "Inter-SemiBold", EAlign::Center, EVAlign::Top);
    
    IRECT titleRect = volumeArea.GetFromTop(30);
    mGraphics->AttachControl(new ITextControl(titleRect, "MASTER", sectionText));
    
    IRECT knobRect = volumeArea.GetReducedFromTop(40).GetCentredInside(100, 100);
    
    auto volumeAction = [this](IControl* pCaller) {
        float value = pCaller->GetValue();
        mEngine->SetMasterVolume(value);
    };
    
    mGraphics->AttachControl(new IVKnobControl(knobRect, volumeAction, "Volume", 
                                               DEFAULT_STYLE, true, 0.0f, 1.0f, 0.8f));
}

void OSMPGui::CreateInfoSection() {
    const IRECT bounds = mGraphics->GetBounds();
    IRECT infoArea = bounds.GetGridCell(0, 2, 2, 3).GetPadded(-kPadding);
    
    IText infoText = IText(14, COLOR_GRAY, "Inter-Light", EAlign::Center, EVAlign::Middle);
    
    auto infoControl = new ITextControl(infoArea, "Load samples via host or drag & drop", infoText);
    mGraphics->AttachControl(infoControl);
}

} // namespace OpenSampler
