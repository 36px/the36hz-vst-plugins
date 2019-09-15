set(plug_sources
	include/NoisemakerController.h
	include/NoisemakerIds.h
	include/NoisemakerProcessor.h
	include/NoisemakerWaveSource.h
	include/version.h

	source/NoisemakerFactory.cpp
	source/NoisemakerController.cpp
	source/NoisemakerProcessor.cpp
	source/NoisemakerWaveSource.cpp
)

set(target NoiseMaker)

smtg_add_vst3plugin(${target} ${plug_sources})
set_target_properties(${target} PROPERTIES ${SDK_IDE_MYPLUGINS_FOLDER})
target_link_libraries(${target} PRIVATE base sdk)

if(SMTG_MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif(SMTG_WIN)
    target_sources(${target} PRIVATE resource/plug.rc)
endif()
