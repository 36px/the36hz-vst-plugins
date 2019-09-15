
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
				return kResultOk;
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerController::setComponentState(IBStream* state)
			{
				return kResultOk;
			}


		} // namespace
	} // namespace
} // namespace 
