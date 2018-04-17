#ifndef CHROMEDINO_HPP
#define CHROMEDINO_HPP

#include <windows.h>
#include <d2d1.h>
#include <Dwrite.h>

#include "StepTimer.h"
#include "Logic.h"

//Base address of dos module, same as the address of the current instance
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class ChromeDino {
    public:
	    ChromeDino();
	    ~ChromeDino();

	    HRESULT	initialize();
	    ID2D1Factory* getDirect2dFactory() const;

	    void runGameLoop();

	private:
		HWND				   _hwnd;
		ID2D1Factory*		   _direct2dFactory;
		ID2D1HwndRenderTarget* _renderTarget;
		IDWriteFactory*		   _writeFactory;
		IDWriteTextFormat*	   _textFormat;
		StepTimer			   _timer;
		Logic				   _logic;

		HRESULT	createDeviceIndependantResources();
		HRESULT	createDeviceResources();

		void discardDeviceResources();

		HRESULT	onRender();

		void onUpdate(const StepTimer& timer);
		void onResize(UINT width, UINT height);

		static LRESULT CALLBACK	wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};


#endif //CHROMEDINO_HPP