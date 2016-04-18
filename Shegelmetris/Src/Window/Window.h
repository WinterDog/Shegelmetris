#pragma once

#include <windows.h>

namespace wd
{
	class Input;

	//-----------------------------------------------------------------------------
	// Windows' window.
	//-----------------------------------------------------------------------------
	class Window
	{
	private:
		// Singleton pointer.
		static Window*					m_pWindow;

		// Class doesn't own it!
		wd::Input*						m_pInputDevice;

		// Created window handle.
		HWND							m_hWnd;
		HDC								m_pDeviceContext;

		// Input console params - save to use them later.
		int								m_nCmdShow;

		// Window size.
		std::uint16_t					m_width;
		std::uint16_t					m_height;

	public:
		Window(
			void);

		~Window(
			void);

	public:
		static Window* Get(
			void);

		// Register class and create window.
		bool Initialize(
			const HINSTANCE& hInstance,
			const int& nCmdShow,
			const std::uint16_t& width,
			const std::uint16_t& height);

		// Get HWND of the created window.
		const HWND& GetHWnd(
			void) const;

		// Get HDC of the created window.
		HDC GetDeviceContext(
			void) const;

		void GetSize(
			std::uint16_t& width,
			std::uint16_t& height) const;

		// Set InputDevice object for processing messages.
		void SetInputDevice(
			wd::Input* const pInputDevice);

		wd::Input* GetInputDevice(
			void) const;

		// Display window.
		void Show(
			void) const;

	private:
		// Method to process initial messages and readdress them to pInputDevice's method.
		static LRESULT CALLBACK WindowProc(
			HWND hWindow,
			UINT message,
			WPARAM wParam,
			LPARAM lParam);
	};
};
