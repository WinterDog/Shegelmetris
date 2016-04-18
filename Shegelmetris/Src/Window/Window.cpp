#include "inc/stdafx.h"

#include "Window.h"

#include "Application/Application.h"
#include "Input/Input.h"

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------

wd::Window* wd::Window::m_pWindow = nullptr;

//-----------------------------------------------------------------------------
// Constructors and destructors
//-----------------------------------------------------------------------------

wd::Window::Window(
	void)
	:
	m_pInputDevice(nullptr)
{
	assert(!m_pWindow);
	m_pWindow = this;
}

wd::Window::~Window(
	void)
{
	m_pWindow = nullptr;
}

//-----------------------------------------------------------------------------
// Static methods
//-----------------------------------------------------------------------------

wd::Window* wd::Window::Get(
	void)
{
	return m_pWindow;
}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

bool wd::Window::Initialize(
	const HINSTANCE& hInstance,
	const int& nCmdShow,
	const std::uint16_t& width,
	const std::uint16_t& height)
{
	m_nCmdShow = nCmdShow;
	m_width = width;
	m_height = height;

	// Register class

	// Create window description.
	WNDCLASSEX wcex;
	// Clear the memory (fill it with zeros).
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);

	// Icon.
	wcex.hIcon = (HICON)::LoadImage(
		nullptr,
		"icon.ico",
		IMAGE_ICON,
		0,
		0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

	assert(wcex.hIcon);
	wcex.hIconSm = wcex.hIcon;

	wcex.hInstance = hInstance;
	// Name of the class
	wcex.lpszClassName = "ShegelmetrisClass";
	// Redraw window at vertical and horizontal moving.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// Assign the static WindowProc() as message processing method.
    wcex.lpfnWndProc = &WindowProc;
    // Reserve space to store the instance pointer.
    wcex.cbWndExtra = sizeof(wd::Window*);

	if (!::RegisterClassEx(&wcex))
	{
		// WinAPI RegisterClassEx() call failed.
		assert(false);
		return false;
	}

	// <<

	// >> Create window

	const DWORD windowStyle = WS_OVERLAPPEDWINDOW;

	// Define window size by required client size.
	RECT rc = { 0, 0, m_width, m_height };
	::AdjustWindowRectEx(&rc, windowStyle, false, NULL);

	// Create window and save its handle to class var.
	m_hWnd = ::CreateWindowEx(
		NULL,
		// Name of the class
		"ShegelmetrisClass",
		// Title
		"Shegelmetris",
		// Window style
		windowStyle,
		// X position
		0,//CW_USEDEFAULT,
		// Y position
		0,//CW_USEDEFAULT,
		// Width
		rc.right - rc.left,
		// Height
		rc.bottom - rc.top,
		// No parent window
		nullptr,
		// No menus
		nullptr,
		// Application handle
		hInstance,
		// Here we save the pointer to [this] to some magic place - we'll need this later.
		static_cast<LPVOID>(this));

	// If window wasn't created, return with error.
	if (!m_hWnd)
	{
		// WinAPI CreateWindowEx() call failed.
		assert(false);
		return false;
	}

	// <<

	m_pDeviceContext = ::GetDC(m_hWnd);

	return true;
}

const HWND& wd::Window::GetHWnd(
	void) const
{
	return m_hWnd;
}

HDC wd::Window::GetDeviceContext(
	void) const
{
	return m_pDeviceContext;
}

void wd::Window::GetSize(
	std::uint16_t& width,
	std::uint16_t& height) const
{
	width = m_width;
	height = m_height;
}

void wd::Window::SetInputDevice(
	Input* const pInputDevice)
{
	assert(!m_pInputDevice);

	m_pInputDevice = pInputDevice;
}

wd::Input* wd::Window::GetInputDevice(
	void) const
{
	return m_pInputDevice;
}

void wd::Window::Show(
	void) const
{
	// Result is not error code or something.
	::ShowWindow(m_hWnd, m_nCmdShow);
}

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------

// Called every time the application receives a message.
LRESULT CALLBACK wd::Window::WindowProc(
	HWND hWindow,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	// This block should be executed once.
	// Here we save instance pointer while handling the first message.
	// EDIT: There are some messages at the creation before WM_NCCREATE,
	// so there's high possibility that we'll pass this without setting pThis to hWindow.
	// We can make static var instead of this check, so there would be no need in double "if" below.
	// But for now let's leave this solution.
	if (message == WM_NCCREATE)
	{
		CREATESTRUCT* const pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		const LPVOID pWindow = pCreateStruct->lpCreateParams;
		::SetWindowLongPtrW(hWindow, 0, reinterpret_cast<LONG_PTR>(pWindow));
	}

	// At this point the instance pointer will always be available.
	auto* const pWindow = reinterpret_cast<wd::Window*>(::GetWindowLongPtrW(hWindow, 0));

	// If we have window and input device pointers, we readdress the message to the InputDevice object.
	if ((pWindow) && (pWindow->m_pInputDevice))
		return pWindow->m_pInputDevice->ProcessEvent(hWindow, message, wParam, lParam);

	// Otherwise call default method.
	return ::DefWindowProc(hWindow, message, wParam, lParam);
}
