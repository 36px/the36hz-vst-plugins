#pragma once

#include "public.sdk/source/vst/vstcomponent.h"

namespace Steinberg {
	namespace Xushifu {
		namespace Noisemaker {

			enum NoisemakerParams : Vst::ParamID {
				kBypassId = 100,
			};

			static const FUID MyProcessorUID(0x1D049745, 0x348E4E8A, 0x8D03B63F, 0x104980AC);
			static const FUID MyControllerUID(0xA9290C45, 0xD1424A5B, 0xA57675B7, 0x1D9828FE);
		}
	}
}
