
#include "../include/NoisemakerProcessor.h"
#include "../include/NoisemakerIds.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

namespace Steinberg {
	namespace Xushifu {
		namespace Noisemaker {


			//------------------------------------------------------------------------
			NoisemakerProcessor::NoisemakerProcessor() : mWaveTable(44100)
			{
				setControllerClass(MyControllerUID);

				//	mWaveTable.init(WaveTable::WaveType::SIN);
				//	mWaveTable.init(WaveTable::WaveType::PULSE);
				mWaveTable.init(WaveTable::WaveType::TRIANGLE);
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerProcessor::initialize(FUnknown* context)
			{
				tresult result = AudioEffect::initialize(context);
				if (result != kResultTrue)
					return kResultFalse;

				addEventInput(STR16("Midi In"));
				addAudioOutput(STR16("Audio Out"), Vst::SpeakerArr::kStereo);

				return kResultTrue;
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerProcessor::setBusArrangements(Vst::SpeakerArrangement* inputs,
				int32 numIns,
				Vst::SpeakerArrangement* outputs,
				int32 numOuts)
			{
				if (numIns == 1 && numOuts == 1) {
					return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
				}
				return kResultFalse;
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerProcessor::setupProcessing(Vst::ProcessSetup& setup)
			{
				KeyState::initKeyStateTable(setup.sampleRate, mKeyStateArray, keyStateArraySize);
				return AudioEffect::setupProcessing(setup);
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerProcessor::setActive(TBool state)
			{
				return AudioEffect::setActive(state);
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerProcessor::process(Vst::ProcessData& data)
			{
				// if (data.inputParameterChanges) {}

				if (data.inputEvents) {
					processEvents(data.inputEvents);
				}

				if (data.numOutputs == 0) {
					return kResultOk;
				}

				if (data.numSamples > 0) {
					processWaveOutput(data);
				}

				return kResultOk;
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerProcessor::setState(IBStream* state)
			{
				return kResultOk;
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerProcessor::getState(IBStream* state)
			{
				return kResultOk;
			}


			//------------------------------------------------------------------------
			void NoisemakerProcessor::processWaveOutput(Vst::ProcessData& data) {

				int  num_samples = data.numSamples;
				float* ch0 = data.outputs[0].channelBuffers32[0];
				int num_channels = data.outputs[0].numChannels;

				if (num_channels < 1) {
					return;
				}

				WaveTable* wave_table = &mWaveTable;

				for (int iKey = 0; iKey < keyStateArraySize; ++iKey) {
					KeyState* state = mKeyStateArray + iKey;
					if (iKey == 0) {
						for (int iSample = 0; iSample < num_samples; ++iSample) {
							ch0[iSample] = 0;
						}
					}
					else if (!state->isNoteOn()) {
						continue;
					}
					else {
						for (int iSample = 0; iSample < num_samples; ++iSample) {
							ch0[iSample] += state->getNextSimple(wave_table);
						}
					}
				}

				for (int iChl = 1; iChl < num_channels; ++iChl) {
					float* ch1 = data.outputs[0].channelBuffers32[iChl];
					for (int iSample = num_samples - 1; iSample >= 0; --iSample) {
						ch1[iSample] = ch0[iSample];
					}
				}

			}


			//------------------------------------------------------------------------
			void NoisemakerProcessor::processEvents(Vst::IEventList* eventlist) {
				Vst::Event e;
				int count = eventlist->getEventCount();
				for (int i = 0; i < count; ++i) {
					tresult result = eventlist->getEvent(i, e);
					if (result == kResultOk) {
						processEvent(e);
					}
				}
			}


			//------------------------------------------------------------------------
			void NoisemakerProcessor::processEvent(const Vst::Event& e) {
				if (e.type == Vst::Event::kNoteOnEvent) {
					int note = e.noteOn.pitch;
					float speed = e.noteOn.velocity;
					getKeyStateByNote(note)->setNoteOn(speed);
				}
				else if (e.type == Vst::Event::kNoteOffEvent) {
					int note = e.noteOff.pitch;
					getKeyStateByNote(note)->setNoteOff();
				}
			}


			//------------------------------------------------------------------------
			KeyState* NoisemakerProcessor::getKeyStateByNote(int note) {
				if ((0 < note) && (note < 128)) {
					return  mKeyStateArray + note;
				}
				else {
					return  &mKeyStateNop;
				}
			}


		} // namespace
	} // namespace
} // namespace 
