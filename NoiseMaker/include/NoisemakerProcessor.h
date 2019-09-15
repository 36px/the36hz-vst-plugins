#pragma once

#include "../include/NoisemakerWaveSource.h"

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstevents.h"

namespace Steinberg {
	namespace Xushifu {
		namespace Noisemaker {

			class NoisemakerProcessor : public Vst::AudioEffect {
			public:
				NoisemakerProcessor();

				tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
				tresult PLUGIN_API setBusArrangements(Vst::SpeakerArrangement* inputs, int32 numIns,
					Vst::SpeakerArrangement* outputs, int32 numOuts) SMTG_OVERRIDE;

				tresult PLUGIN_API setupProcessing(Vst::ProcessSetup& setup) SMTG_OVERRIDE;
				tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
				tresult PLUGIN_API process(Vst::ProcessData& data) SMTG_OVERRIDE;

				tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
				tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;

				static FUnknown* createInstance(void*) {
					Vst::IAudioProcessor* processor = new NoisemakerProcessor();
					return processor;
				}

			private:

				static	const int  keyStateArraySize = 128;

				WaveTable mWaveTable;
				KeyState  mKeyStateArray[keyStateArraySize];
				KeyState  mKeyStateNop;

				bool mBypass = false;

			private:
				void processEvents(Vst::IEventList* eventlist);
				void processEvent(const Vst::Event& e);
				void processWaveOutput(Vst::ProcessData& data);
				KeyState* getKeyStateByNote(int note);
			};

		} // namespace
	} // namespace
} // namespace
