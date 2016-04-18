#pragma once

#include "HighResClock/HighResClock.h"

// HINSTANCE
#include <windows.h>

namespace wd
{
	class Input;
	class Scene;
	class VideoDevice;
	class Window;
	struct Config;

	//-----------------------------------------------------------------------------
	// Application class.
	// Instance of this is created at the Main() method.
	// Initialization of engine objects like video device, window and so on happens here,
	// game loop starts and runs here too.
	//-----------------------------------------------------------------------------
	class Application
	{
	private:
		// How much time should pass between logic ticks.
		static const double								m_frameTime;
		// Singleton pointer.
		static Application*								m_pApplication;

		// Used to perform logick ticks only 60 times per second
		// and get the same speed on any machine.
		wd::HighResClock								m_highResClock;

		// Settings of the application.
		std::unique_ptr<wd::Config>						m_pConfig;
		// Window.
		std::unique_ptr<wd::Window>						m_pWindow;
		// Input device.
		std::unique_ptr<wd::Input>						m_pInputDevice;
		// Video device.
		std::unique_ptr<wd::VideoDevice>				m_pVideoDevice;
		// Scene.
		std::unique_ptr<wd::Scene>						m_pScene;

	public:
		Application(
			void);

		~Application(
			void);

	public:
		static Application* Get(
			void);

		// Called from Main() method.
		// Creates engine stuff.
		bool Initialize(
			const HINSTANCE& hInstance,
			const int& nCmdShow);

		wd::Window* GetWindow(
			void) const;

		// Launch global application loop.
		// Returns exit message code when the application is closed.
		int BeginGlobalLoop(
			void);

	private:
		// Init methods.

		void InitConfig(
			void);

		bool InitWindow(
			const HINSTANCE& hInstance,
			const int& nCmdShow);

		bool InitVideoDevice(
			void);

		void InitInput(
			void);

		void InitScene(
			void);
	};
};
