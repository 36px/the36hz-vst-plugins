
#include "../include/NoisemakerController.h"
#include "../include/NoisemakerIds.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"

namespace Steinberg {
	namespace Xushifu {
		namespace  Noisemaker {


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerController::initialize(FUnknown* context)
			{
				tresult result = EditController::initialize(context);
				if (result == kResultTrue)
				{
					parameters.addParameter(STR16("Bypass"), nullptr, 1, 0,
						Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass,
						NoisemakerParams::kBypassId);

					parameters.addParameter(STR16("Volume"), STR16("dB"), 100, 0,
						Vst::ParameterInfo::kCanAutomate, NoisemakerParams::kVolume, 0,
						STR16("Volume"));

					parameters.addParameter(STR16("Wave Type"), nullptr, 3, 1,
						Vst::ParameterInfo::kCanAutomate, NoisemakerParams::kWaveType, 0,
						STR16("Wave Type"));
				}
				return kResultTrue;
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerController::setComponentState(IBStream* state)
			{
				if (!state)
					return kResultFalse;

				IBStreamer streamer(state, kLittleEndian);

				int32 bypassState;
				if (streamer.readInt32(bypassState) == false)
					return kResultFalse;
				setParamNormalized(NoisemakerParams::kBypassId, bypassState ? 1 : 0);

				float volume = 0.0f;
				if (streamer.readFloat(volume) == false)
					return kResultFalse;
				setParamNormalized(NoisemakerParams::kVolume, volume);

				int32  waveType = 0;
				if (streamer.readInt32(waveType) == false)
					return kResultFalse;
				setParamNormalized(NoisemakerParams::kWaveType, waveType);

				return kResultOk;
			}


		} // namespace
	} // namespace
} // namespace 
