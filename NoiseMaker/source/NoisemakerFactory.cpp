
#include "public.sdk/source/main/pluginfactory.h"

#include "../include/NoisemakerController.h"
#include "../include/NoisemakerProcessor.h"
#include "../include/NoisemakerIds.h"
#include "../include/version.h"

#define stringSubCategory  Steinberg::Vst::PlugType::kInstrument

BEGIN_FACTORY_DEF(stringCompanyName, stringCompanyWeb, stringCompanyEmail)

DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Xushifu::Noisemaker::MyProcessorUID),
	PClassInfo::kManyInstances,
	kVstAudioEffectClass,
	stringPluginName,
	Vst::kDistributable,
	stringSubCategory,
	FULL_VERSION_STR,
	kVstVersionString,
	Steinberg::Xushifu::Noisemaker::NoisemakerProcessor::createInstance)

DEF_CLASS2(INLINE_UID_FROM_FUID(Steinberg::Xushifu::Noisemaker::MyControllerUID) ,
	PClassInfo::kManyInstances,
	kVstComponentControllerClass,
	stringPluginName "Controller",
	0,
	"",
	FULL_VERSION_STR,
	kVstVersionString,
	Steinberg::Xushifu::Noisemaker::NoisemakerController::createInstance)

END_FACTORY

bool InitModule() {
	return true;
}

bool DeinitModule() {
	return true;
}
