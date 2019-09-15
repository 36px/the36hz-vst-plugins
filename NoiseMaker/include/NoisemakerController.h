#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

namespace Steinberg {
	namespace Xushifu {
		namespace Noisemaker {

			class NoisemakerController : public Vst::EditController {
			public:
				static FUnknown* createInstance(void*) {
					Vst::IEditController* controller = new NoisemakerController();
					return controller;
				}

				tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;

				tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
			};
		}
	}
}
