#pragma once

namespace Steinberg {
	namespace Xushifu {
		namespace Noisemaker {

			class  WaveFunction {
			public:
				WaveFunction() {}
				~WaveFunction() {}
				virtual  double valueOf(double pos) = 0;
			};


			class  WaveTable {
			private:
				float* mBuffer;
				const int mBufferLength;

			public:

				enum WaveType { UNDEFINE, SIN, PULSE, TRIANGLE };

				static const double  oneCircuit;

				WaveTable(int buffer_length);
				~WaveTable();
				void  init(WaveType type);
				void  init(WaveFunction* func);
				float getValue(double  pos);
			};


			class KeyState {

			private:
				int mIndex;
				int mNote;
				double mFrequency;
				double mPosition;
				double mStepLength;
				float  mVolume;

			public:
				KeyState();
				float  getNextSimple(WaveTable* wave_table);
				void init(int index, int note);
				void config(int sample_rate, double frequency);
				void reset();
				void setNoteOn(float speed);
				void setNoteOff();
				bool isNoteOn();

				static void  initKeyStateTable(int   sample_rate, KeyState* array, int count);
			};

		} // namespace
	} // namespace
} // namespace
