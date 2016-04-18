#include "inc/stdafx.h"

#include "Application.h"

#include "Config/Config.h"
#include "Input/Input.h"
#include "Scene/Scene.h"
#include "VideoDevice/VideoDevice.h"
#include "Window/Window.h"

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------

// 60 frames per second.
const double wd::Application::m_frameTime = 1.0 / 60.0;
// Init static pointer.
wd::Application* wd::Application::m_pApplication = nullptr;

//-----------------------------------------------------------------------------
// Constructors and destructors
//-----------------------------------------------------------------------------

wd::Application::Application(
	void)
{
	assert(!m_pApplication);
	m_pApplication = this;
}

// Release memory.
wd::Application::~Application(
	void)
{
	m_pApplication = nullptr;
}

//-----------------------------------------------------------------------------
// Static methods
//-----------------------------------------------------------------------------

wd::Application* wd::Application::Get(
	void)
{
	return m_pApplication;
}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

bool wd::Application::Initialize(
	const HINSTANCE& hInstance,
	const int& nCmdShow)
{
	// Init config file - get global settings.
	InitConfig();

	// Create OS window.
	if (!InitWindow(hInstance, nCmdShow))
		return false;

	// Create video device and context.
	if (!InitVideoDevice())
		return false;

	InitInput();
	// Show window. By now we should be able to show some sort of loading screen.
	m_pWindow->Show();
	// Create scene.
	InitScene();

	return true;
}

wd::Window* wd::Application::GetWindow(
	void) const
{
	return m_pWindow.get();
}

int wd::Application::BeginGlobalLoop(
	void)
{
	MSG message = { 0 };

	// Timer to keep stable 60 frames per second.
	m_highResClock.Reset();

	// Main message loop.
	while (message.message != WM_QUIT)
	{
		if (::PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			// Translate message into the right format.
			::TranslateMessage(&message);
			// Send the message to WndProc().
			::DispatchMessage(&message);
			// If there was a message from OS, we should check - maybe, there are more.
			// So we skip our internal tick this iteration.
			continue;
		}

		// Time passed since the last Reset().
		const double elapsedTime = m_highResClock.GetElapsedSec();
		// If enough time has passed...
		if (elapsedTime >= m_frameTime)
		{
			// Process logic and rendering.
			m_pScene->StepLogic(m_frameTime);
			m_pScene->Draw(m_frameTime);
			// Reset timer.
			m_highResClock.Reset();
		}
	}
	return static_cast<int>(message.wParam);
}

//-----------------------------------------------------------------------------
// Protected methods
//-----------------------------------------------------------------------------

// Temp method for creating scene from a file.
void wd::Application::InitConfig(
	void)
{
	m_pConfig = std::make_unique<wd::Config>("Config.xml");
}

bool wd::Application::InitWindow(
	const HINSTANCE& hInstance,
	const int& nCmdShow)
{
	// Create inner window object.
	m_pWindow = std::make_unique<wd::Window>();

	// Initialize the window.
	const bool success = m_pWindow->Initialize(
		hInstance,
		nCmdShow,
		m_pConfig->resolution.x,
		m_pConfig->resolution.y);
	if (!success)
	{
		// Couldn't create window.
		assert(false);
		return false;
	}

	return true;
}

bool wd::Application::InitVideoDevice(
	void)
{
	// Create inner video device object.
	m_pVideoDevice = std::make_unique<wd::VideoDevice>();

	// Initialize video device.
	if (!m_pVideoDevice->Initialize(
		m_pWindow->GetHWnd(),
		m_pWindow->GetDeviceContext(),
		m_pConfig->resolution,
		m_pConfig->fieldSize,
		m_pConfig->marginLeft,
		m_pConfig->marginTop,
		m_pConfig->marginBottom))
	{
		// Couldn't create video device.
		assert(false);
		return false;
	}

	return true;
}

void wd::Application::InitInput(
	void)
{
	// Create inner input messages processing device object.
	m_pInputDevice = std::make_unique<wd::Input>();

	// Set messages processing device for the window.
	m_pWindow->SetInputDevice(m_pInputDevice.get());
}

// Temp method for creating scene from a file.
void wd::Application::InitScene(
	void)
{
	m_pScene = std::make_unique<wd::Scene>(
		m_pConfig->fieldSize,
		m_pVideoDevice.get());

	m_pScene->SetStartingSpeed(m_pConfig->startingSpeed);
	m_pScene->SetSpeedIncrement(m_pConfig->speedIncrement);
}
