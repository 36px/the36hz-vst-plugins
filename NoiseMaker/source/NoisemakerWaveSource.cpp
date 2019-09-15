
#include "../include/NoisemakerWaveSource.h"

#include <math.h>

namespace Steinberg {
	namespace Xushifu {
		namespace Noisemaker {


			class SinFunc : public WaveFunction {
			public:
				double valueOf(double pos) {
					return sin(pos);
				}
			};


			class  PulseFunc : public WaveFunction {
			public:
				double valueOf(double pos) {
					const double pi = 3.1415926;
					double times = pos / (2 * pi);
					double phase = times - floor(times);
					float  vol = (phase < 0.5) ? 1 : -1;
					return  vol * 0.8;
				}
			};


			class TriangleFunc : public WaveFunction {
			public:
				double valueOf(double pos) {
					const double pi = 3.1415926;
					double times = pos / (2 * pi);
					double x = times - floor(times);
					double y = 0;
					if (x < 0.25) { y = 4 * x; }
					else if (x > 0.75) { y = (4 * x) - 4; }
					else { y = 2 - (4 * x); }
					return   0.8 * y;
				}
			};


			/**************************************
			 *
			 * class WaveTable
			 *
			 **************************************/


			const	double WaveTable::oneCircuit = 3.1415926 * 2;


			//------------------------------------------------------------------------
			WaveTable::WaveTable(int size) :mBufferLength(size) {
				mBuffer = new float[size];
			}


			//------------------------------------------------------------------------
			WaveTable:: ~WaveTable() {
				delete[] mBuffer;
			}


			//------------------------------------------------------------------------
			void WaveTable::init(WaveTable::WaveType type) {

				TriangleFunc triangle;
				PulseFunc pulse;
				SinFunc  sin_func;

				switch (type) {
				case  WaveType::PULSE:
					init(&pulse);
					break;
				case  WaveType::TRIANGLE:
					init(&triangle);
					break;
				case  WaveType::SIN:
				default:
					init(&sin_func);
					break;
				}
			}


			//------------------------------------------------------------------------
			void  WaveTable::init(WaveFunction* func) {
				for (int i = 0; i < mBufferLength; ++i) {
					double pos = (i * oneCircuit) / mBufferLength;
					mBuffer[i] = func->valueOf(pos);
				}
			}


			//------------------------------------------------------------------------
			float WaveTable::getValue(double  pos) {
				int index = (int)((pos * mBufferLength) / oneCircuit);
				index = (index < 0) ? 0 : index;
				return  mBuffer[index % mBufferLength];
			}


			/**************************************
			 *
			 * class KeyState
			 *
			 **************************************/


			 //------------------------------------------------------------------------
			KeyState::KeyState() : mVolume(-1), mIndex(0), mNote(0), mPosition(0), mStepLength(0), mFrequency(1) {
			}


			//------------------------------------------------------------------------
			void KeyState::initKeyStateTable(int sample_rate, KeyState* array, int count) {

				int     note = 69; // the note 'A0'
				double  freq = 440;
				const double root12of2 = 1.059463094;

				// go up
				for (; note < 128; ) {
					note += 12;
					freq *= 2;
				}

				// go down
				for (; ; note -= 12, freq /= 2) {
					if (note < 0) {
						break;
					}
					else if (127 < note) {
						continue;
					}

					double freq2 = freq;
					for (int offset = 0; offset < 12; ++offset, freq2 *= root12of2) {
						int note2 = note + offset;
						if ((0 <= note2) && (note2 < count)) {
							array[note2].config(sample_rate, freq2);
						}
					}
				}
			}


			//------------------------------------------------------------------------
			float KeyState::getNextSimple(WaveTable* wave_table) {
				double  pos = mPosition;
				float   value = wave_table->getValue(pos);
				mPosition = pos + mStepLength;
				return  value * mVolume;
			}


			//------------------------------------------------------------------------
			void KeyState::init(int index, int note) {
				mIndex = index;
				mNote = note;
			}


			//------------------------------------------------------------------------
			void  KeyState::config(int sample_rate, double frequency) {
				mFrequency = frequency;
				mStepLength = WaveTable::oneCircuit * frequency / sample_rate;
			}


			//------------------------------------------------------------------------
			void KeyState::reset() {
				mVolume = -1;
			}


			//------------------------------------------------------------------------
			void KeyState::setNoteOn(float speed) {
				mPosition = 0;
				mVolume = speed;
			}


			//------------------------------------------------------------------------
			void KeyState::setNoteOff() {
				mVolume = -1;
			}


			//------------------------------------------------------------------------
			bool KeyState::isNoteOn() {
				return(mVolume > 0);
			}


		} // namespace
	} // namespace
} // namespace
