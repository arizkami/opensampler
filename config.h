#ifndef _OPENSAMPLER_CONFIG_H_
#define _OPENSAMPLER_CONFIG_H_

#define PLUG_NAME "OpenSampler"
#define PLUG_MFR "OpenSampler"
#define PLUG_VERSION_HEX 0x00010000
#define PLUG_VERSION_STR "1.0.0"
#define PLUG_UNIQUE_ID 'OSmP'
#define PLUG_MFR_ID 'OSmp'
#define PLUG_URL_STR "https://github.com/arizkami/opensampler"
#define PLUG_EMAIL_STR "info@opensampler.app"
#define PLUG_COPYRIGHT_STR "Copyright 2026"
#define PLUG_CLASS_NAME OpenSampler

#define BUNDLE_NAME "OpenSampler"
#define BUNDLE_MFR "OpenSampler"
#define BUNDLE_DOMAIN "com"

#define PLUG_CHANNEL_IO "0-2"
#define PLUG_LATENCY 0
#define PLUG_TYPE 1
#define PLUG_DOES_MIDI_IN 1
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 1
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 800
#define PLUG_HEIGHT 600
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 0

#define AUV2_ENTRY OpenSampler_Entry
#define AUV2_ENTRY_STR "OpenSampler_Entry"
#define AUV2_FACTORY OpenSampler_Factory
#define AUV2_VIEW_CLASS OpenSampler_View
#define AUV2_VIEW_CLASS_STR "OpenSampler_View"

#define AAX_TYPE_IDS 'IPI1', 'IPI2'
#define AAX_PLUG_MFR_STR "OpenSampler"
#define AAX_PLUG_NAME_STR "OpenSampler\nIPIS"
#define AAX_PLUG_CATEGORY_STR "Synth"
#define AAX_DOES_AUDIOSUITE 0

#define VST3_SUBCATEGORY "Instrument|Sampler"
#define VST3_PRESET_LIST "Default"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64

#define INTER_REGULAR_FN "Graphic/Resources/Fonts/Inter/Inter-Regular.otf"
#define INTER_BOLD_FN "Graphic/Resources/Fonts/Inter/Inter-Bold.otf"
#define INTER_SEMIBOLD_FN "Graphic/Resources/Fonts/Inter/Inter-SemiBold.otf"
#define INTER_MEDIUM_FN "Graphic/Resources/Fonts/Inter/Inter-Medium.otf"
#define INTER_LIGHT_FN "Graphic/Resources/Fonts/Inter/Inter-Light.otf"

#endif
