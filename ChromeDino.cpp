#include <iostream>

#include "ChromeDino.h"
#include "Input.h"
#include "Resolution.h"
#include "Utils.h"

using namespace D2D1;

/// <summary>
/// Constructor
/// </summary>
ChromeDino::ChromeDino() :
	_hwnd			 (nullptr),
	_direct2dFactory (nullptr),
	_renderTarget	 (nullptr), 
	_writeFactory	 (nullptr), 
	_textFormat	     (nullptr),
	_logic			 (this) {}

/// <summary>
/// Destructor
/// </summary>
ChromeDino::~ChromeDino() {
	Utils::safeRelease(&_direct2dFactory);
	Utils::safeRelease(&_renderTarget);
}

/// <summary>
/// Initializes the window and resources of the game
/// </summary>
/// <returns>HRESULT</returns>
HRESULT ChromeDino::initialize() {

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	auto hr = createDeviceIndependantResources();

	if (SUCCEEDED(hr)) {
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = wndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDI_APPLICATION);
		wcex.lpszClassName = "VSApp";

		RegisterClassEx(&wcex);

		//Calculate the real size of the window for the overlapped style
		RECT rect = { 0, 0, WIDTH, HEIGHT };
		AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

		auto dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);

		// Create the window.
		_hwnd = CreateWindow(
			"VSApp",
			"ChromeDino",
			dwStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);
		hr = _hwnd ? S_OK : E_FAIL;

		//Show the window if it was created
		if (SUCCEEDED(hr)) {
			ShowWindow(_hwnd, SW_SHOWNORMAL);
			UpdateWindow(_hwnd);
		}
	}

	return hr;
}

/// <summary>
/// Runs the message loop of the window
/// </summary>
void ChromeDino::runGameLoop() {
	MSG msg;
	msg.message = WM_NULL;

	_logic.initialize();
	//Get all the messages from this thread's windows
	while(msg.message != WM_QUIT) {
		//Check for message
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			auto wasHandled = false;
			//Handle all keyboard messages
			wasHandled = Input::getInstance().tryHandleKeyboardMessage(msg);

			if (!wasHandled) {
				TranslateMessage(&msg);
				//Dispatch the message to the callback
				DispatchMessage(&msg);
			}
		} else {
			if(Input::getInstance().isKeyDown(Input::Escape)) {
				break;
			}

			//Update and render the game
			_timer.Tick([&]() {
				onUpdate(_timer);
			});

			onRender();
		}
	}
}

/// <summary>
/// Returns the direct2d factory
/// </summary>
/// <returns></returns>
ID2D1Factory* ChromeDino::getDirect2dFactory() const {
	return _direct2dFactory;
}

/// <summary>
/// Creates the device independant resources such as the factory
/// </summary>
/// <returns>HRESULT</returns>
HRESULT ChromeDino::createDeviceIndependantResources() {
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_direct2dFactory);

	static const WCHAR mscFontName[] = L"Verdana";
	static const FLOAT mscFontSize = 20;

	if (SUCCEEDED(hr)) {

		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(_writeFactory),
			reinterpret_cast<IUnknown **>(&_writeFactory)
		);
	}
	if (SUCCEEDED(hr)) {
		// Create a DirectWrite text format object.
		hr = _writeFactory->CreateTextFormat(
			mscFontName,
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			mscFontSize,
			L"", //locale
			&_textFormat
		);
	}
	if (SUCCEEDED(hr)) {
		// Center the text horizontally and vertically.
		_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	return hr;
}

/// <summary>
/// Creates the device resources such as the render target
/// </summary>
/// <returns>HRESULT</returns>
HRESULT ChromeDino::createDeviceResources() {
	HRESULT hr = S_OK;

	if (!_renderTarget) {
		RECT rc;

		//Get the client area of the window (1024x768 if not changed)
		GetClientRect(_hwnd, &rc);

		const D2D1_SIZE_U size = SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = _direct2dFactory->CreateHwndRenderTarget(
			RenderTargetProperties(),
			HwndRenderTargetProperties(_hwnd, size),
			&_renderTarget
		);
	}
	return hr;
}

/// <summary>
/// Releases the device resources
/// </summary>
void ChromeDino::discardDeviceResources() {
	Utils::safeRelease(&_renderTarget);
}

/// <summary>
/// Called to render to the window
/// </summary>
/// <returns></returns>
HRESULT ChromeDino::onRender() {
	HRESULT hr = S_OK;

	hr = createDeviceResources();

	if (SUCCEEDED(hr)) {
		//Begin drawing
		_renderTarget->BeginDraw();

		_renderTarget->SetTransform(Matrix3x2F::Identity());

		//clear the window to white
		_renderTarget->Clear(ColorF(ColorF::LightSkyBlue)); 

		//Render the game
		_logic.onRender(_renderTarget, _textFormat);

		//End drawing
		hr = _renderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET) {
		hr = S_OK;
		discardDeviceResources();
	}

	return hr;
}

/// <summary>
/// Updates the game logic
/// </summary>
/// <param name="timer">Timer to get delta time</param>
void ChromeDino::onUpdate(const StepTimer& timer) {
	auto delta = timer.GetElapsedSeconds();
	if(_logic.onUpdate(delta)) {
		//Game ended
		PostQuitMessage(0);
	}
}

/// <summary>
/// Called when the window resizes
/// </summary>
/// <param name="width">width of the window</param>
/// <param name="height">height of the window</param>
void ChromeDino::onResize(UINT width, UINT height) {
	if (_renderTarget != nullptr) {
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		_renderTarget->Resize(SizeU(width, height));
	}
}

/// <summary>
/// Handles the messages for a window
/// </summary>
/// <param name="hwnd">The window handle</param>
/// <param name="message">The message</param>
/// <param name="w_param">Additional parameters</param>
/// <param name="l_param">Additional parameters</param>
/// <returns></returns>
LRESULT ChromeDino::wndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
	LRESULT result = 0;

	if (message == WM_CREATE) {
		//Store the game instance on creation
		auto pcs = reinterpret_cast<LPCREATESTRUCT>(l_param);
		auto g = static_cast<ChromeDino*>(pcs->lpCreateParams);

		::SetWindowLongPtr(
			hwnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(g)
		);

		result = 1;
	} else {
		//Get the game instance
		ChromeDino* dino = reinterpret_cast<ChromeDino*>(
			::GetWindowLongPtr(
				hwnd,
				GWLP_USERDATA
			));

		auto wasHandled = false;

		if (dino) {
			switch (message) {
				case WM_SIZE:
				{
					//Resizing event
					const UINT width = LOWORD(l_param);
					const UINT height = HIWORD(l_param);
					dino->onResize(width, height);

					result = 0;
					wasHandled = true;
					break;
				}
				case WM_DISPLAYCHANGE:
				{
					//Resolution has changed
					InvalidateRect(hwnd, nullptr, FALSE);

					result = 0;
					wasHandled = true;
					break;

				}
				case WM_DESTROY: 
				{
					//Destroy the window
					PostQuitMessage(0);

					result = 1;
					wasHandled = true;
					break;
				}
			}
		}

		if (!wasHandled) {
			//Handle with default behaviour
			result = DefWindowProc(hwnd, message, w_param, l_param);
		}
	}

	return result;
}

