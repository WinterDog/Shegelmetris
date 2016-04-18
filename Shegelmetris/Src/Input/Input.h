#pragma once

#include <windows.h>

namespace wd
{
	//-----------------------------------------------------------------------------
	// Windows input class - key processing and WM event handling.
	//-----------------------------------------------------------------------------
	class Input
	{
	public:
		// TODO: These actions should not be bound to the input.
		// It should be redesigned to more flexible event-message system.
		enum class Key : std::uint8_t
		{
			// Rotate figure.
			Action = 1 << 0,
			// Start new game.
			StartGame = 1 << 1,
			// Esc.
			Back = 1 << 2,
			// Arrows.
			Left = 1 << 3,
			Right = 1 << 4,
			Down = 1 << 5,
		};

	private:
		// Key is Windows key code and value is internal enum value.
		typedef std::unordered_map<
			UINT_PTR,
			Key>							InputKeyMap;

	private:
		static wd::Input*				m_pInput;

		InputKeyMap							m_inputKeyMap;
		std::uint8_t						m_keysDown;

		double								m_keyPressDelayTimer;

	public:
		Input(
			void);

		~Input(
			void);

	public:
		// Check is the [key] pressed at the moment.
		static bool IsKeyDown(
			const Key& key);

		// Callback for processing window events.
		LRESULT ProcessEvent(
			const HWND& hWindow,
			const UINT& wmCode,
			const WPARAM& wParam,
			const LPARAM& lParam);

	private:
		void WMKeyDown(
			const UINT_PTR& wParam);

		void WMKeyUp(
			const UINT_PTR& wParam);
	};
};
