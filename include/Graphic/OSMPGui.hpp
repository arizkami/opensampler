#ifndef _OSMPGUI_HPP_
#define _OSMPGUI_HPP_

#include "IControl.h"
#include "IControls.h"

namespace OpenSampler {

class OSMPEngine;

class OSMPGui {
public:
    OSMPGui(iplug::igraphics::IGraphics* pGraphics, OSMPEngine* pEngine);
    ~OSMPGui();
    
    void CreateControls();
    void OnResize(int width, int height);
    
private:
    void CreateHeader();
    void CreateEnvelopeSection();
    void CreateVolumeSection();
    void CreateInfoSection();
    
    iplug::igraphics::IGraphics* mGraphics;
    OSMPEngine* mEngine;
    
    static constexpr int kHeaderHeight = 60;
    static constexpr int kPadding = 20;
};

} // namespace OpenSampler

#endif
