//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : plugcontroller.cpp
// Created by  : Steinberg, 01/2018
// Description : HelloWorld Example for VST 3
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2019, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "../include/plugcontroller.h"
#include "../include/plugids.h"
#include "../include/HelloGUI.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"

namespace Steinberg {
	namespace HelloWorld {


		class MfcView : public IPlugView
		{

		private:

			HelloGUI  mView;


			tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) {
				if (_iid == IPlugView::iid) {
					IPlugView* ptr = this;
					*obj = ptr;
					return  kResultOk;
				}
				else {
					return kResultFalse;
				}
			}

			uint32 PLUGIN_API addRef() {
				return 1;
			}

			uint32 PLUGIN_API release() {
				return 1;
			}


			/////////////

			tresult PLUGIN_API isPlatformTypeSupported(FIDString type) {

				return kResultOk;
			}

			tresult PLUGIN_API attached(void* parent, FIDString type) {

				HWND hWnd = (HWND)parent;
				CWnd* pWnd = CWnd::FromHandle(hWnd);
				this->mView.SetParent(pWnd);
				return kResultOk;
			}

			tresult PLUGIN_API removed() {
				return kResultOk;
			}

			tresult PLUGIN_API onWheel(float distance) {
				return kResultOk;
			}

			tresult PLUGIN_API onKeyDown(char16 key, int16 keyCode, int16 modifiers) {
				return kResultOk;
			}

			tresult PLUGIN_API onKeyUp(char16 key, int16 keyCode, int16 modifiers) {
				return kResultOk;
			}

			tresult PLUGIN_API getSize(ViewRect* size) {
				size->bottom = 256;
				size->left = 0;
				size->right = 256;
				size->top = 0;
				return kResultOk;
			}

			tresult PLUGIN_API onSize(ViewRect* newSize) {
				int  h = newSize->getHeight();
				int x = newSize->left;
				int w = newSize->getWidth();
				int y = newSize->top;
				this->mView.MoveWindow(x, y, w, h);
				return kResultOk;
			}

			tresult PLUGIN_API onFocus(TBool state) {
				return kResultOk;
			}

			tresult PLUGIN_API setFrame(IPlugFrame* frame) {
				return kResultOk;
			}

			tresult PLUGIN_API canResize() {
				return kResultTrue;
			}

			tresult PLUGIN_API checkSizeConstraint(ViewRect* rect) {
				return kResultOk;
			}

		};


		//-----------------------------------------------------------------------------
		tresult PLUGIN_API PlugController::initialize(FUnknown* context)
		{
			tresult result = EditController::initialize(context);
			if (result == kResultTrue)
			{
				//---Create Parameters------------
				parameters.addParameter(STR16("Bypass"), 0, 1, 0,
					Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass,
					HelloWorldParams::kBypassId);

				parameters.addParameter(STR16("Parameter 1"), STR16("dB"), 0, .5,
					Vst::ParameterInfo::kCanAutomate, HelloWorldParams::kParamVolId, 0,
					STR16("Param1"));
				parameters.addParameter(STR16("Parameter 2"), STR16("On/Off"), 1, 1.,
					Vst::ParameterInfo::kCanAutomate, HelloWorldParams::kParamOnId, 0,
					STR16("Param2"));
			}
			return kResultTrue;
		}

		//------------------------------------------------------------------------
		tresult PLUGIN_API PlugController::setComponentState(IBStream* state)
		{
			// we receive the current state of the component (processor part)
			// we read our parameters and bypass value...
			if (!state)
				return kResultFalse;

			IBStreamer streamer(state, kLittleEndian);

			float savedParam1 = 0.f;
			if (streamer.readFloat(savedParam1) == false)
				return kResultFalse;
			setParamNormalized(HelloWorldParams::kParamVolId, savedParam1);

			int8 savedParam2 = 0;
			if (streamer.readInt8(savedParam2) == false)
				return kResultFalse;
			setParamNormalized(HelloWorldParams::kParamOnId, savedParam2);

			// read the bypass
			int32 bypassState;
			if (streamer.readInt32(bypassState) == false)
				return kResultFalse;
			setParamNormalized(kBypassId, bypassState ? 1 : 0);

			return kResultOk;
		}

		IPlugView* PLUGIN_API PlugController::createView(FIDString /*name*/) {
			return  new  MfcView();
		}

		//------------------------------------------------------------------------
	} // namespace
} // namespace Steinberg
