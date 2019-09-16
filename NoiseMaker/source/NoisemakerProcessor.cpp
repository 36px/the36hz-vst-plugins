
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
				mWaveTable.init(WaveTable::WaveType::SIN);
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
				if (data.inputParameterChanges) {
					processParamsChange(data);
				}

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
				if (!state)
					return kResultFalse;

				IBStreamer streamer(state, kLittleEndian);

				int32 savedBypass = 0;
				if (streamer.readInt32(savedBypass) == false)
					return kResultFalse;

				float volume = 0.f;
				if (streamer.readFloat(volume) == false)
					return kResultFalse;

				int32 waveTypeInt = 0;
				if (streamer.readInt32(waveTypeInt) == false)
					return kResultFalse;
				WaveTable::WaveType  waveType = (WaveTable::WaveType)  waveTypeInt;

				if (waveType != mWaveType) {
					mWaveTable.init(waveType);
				}

				mVolume = volume;
				mWaveType = waveType;

				return kResultOk;
			}


			//------------------------------------------------------------------------
			tresult PLUGIN_API NoisemakerProcessor::getState(IBStream* state)
			{
				int32 toSaveBypass = mBypass ? 1 : 0;
				float toSaveVolume = mVolume;
				int32 toSaveWaveType = mWaveType;

				IBStreamer streamer(state, kLittleEndian);
				streamer.writeInt32(toSaveBypass);
				streamer.writeFloat(toSaveVolume);
				streamer.writeInt32(toSaveWaveType);

				return kResultOk;
			}


			//-----------------------------------------------------------------------------
			void NoisemakerProcessor::processParamsChange(Vst::ProcessData& data)
			{
				int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
				for (int32 index = 0; index < numParamsChanged; index++)
				{
					Vst::IParamValueQueue* paramQueue =
						data.inputParameterChanges->getParameterData(index);
					if (paramQueue)
					{
						Vst::ParamValue value;
						int32 sampleOffset;
						int32 numPoints = paramQueue->getPointCount();
						switch (paramQueue->getParameterId())
						{
						case  NoisemakerParams::kVolume:
							if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) ==
								kResultTrue)
								mVolume = value;
							break;
						case NoisemakerParams::kWaveType:
							if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) ==
								kResultTrue) {
								int n = (int)(value * 5);
								mWaveType = (WaveTable::WaveType)  n;
								mWaveTable.init(mWaveType);
							}
							break;
						case NoisemakerParams::kBypassId:
							if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) ==
								kResultTrue)
								mBypass = (value > 0.5f);
							break;
						}
					}
				}
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

				for (int iSample = 0; iSample < num_samples; ++iSample) {
					ch0[iSample] = ch0[iSample] * mVolume;
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
