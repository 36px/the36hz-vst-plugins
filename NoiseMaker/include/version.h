#pragma once

#include "pluginterfaces/base/fplatform.h"

#define MAJOR_VERSION_STR "1"
#define MAJOR_VERSION_INT 1

#define SUB_VERSION_STR "0"
#define SUB_VERSION_INT 0

#define RELEASE_NUMBER_STR "2"
#define RELEASE_NUMBER_INT 2

#define BUILD_NUMBER_STR "10"
#define BUILD_NUMBER_INT 10


#define FULL_VERSION_STR MAJOR_VERSION_STR "." SUB_VERSION_STR "." RELEASE_NUMBER_STR "." BUILD_NUMBER_STR


#define VERSION_STR MAJOR_VERSION_STR "." SUB_VERSION_STR "." RELEASE_NUMBER_STR


#define stringPluginName		"NoiseMaker"

#define stringOriginalFilename	"NoiseMaker.vst3"
#if SMTG_PLATFORM_64
#define stringFileDescription	stringPluginName" VST3-SDK (64Bit)"
#else
#define stringFileDescription	stringPluginName" VST3-SDK"
#endif
#define stringCompanyName		"Xu Shi Fu\0"
#define stringCompanyWeb		"https://space.bilibili.com/22370212"
#define stringCompanyEmail		"mailto:36px_net@sina.com"

#define stringLegalCopyright	"?2019 Xu Shi Fu"
#define stringLegalTrademarks	"VST is a trademark of Steinberg Media Technologies GmbH"
