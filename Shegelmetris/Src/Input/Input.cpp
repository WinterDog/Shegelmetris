#include "inc/stdafx.h"

#include "Input.h"

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------

wd::Input* wd::Input::m_pInput = nullptr;

//-----------------------------------------------------------------------------
// Constructors and destructors
//-----------------------------------------------------------------------------

wd::Input::Input(
	void)
	:
	m_keysDown(0)
{
	assert(!m_pInput);
	m_pInput = this;

	m_inputKeyMap.insert({ VK_RETURN, wd::Input::Key::StartGame });
	m_inputKeyMap.insert({ VK_SPACE, wd::Input::Key::Action });
	m_inputKeyMap.insert({ VK_ESCAPE, wd::Input::Key::Back });

	m_inputKeyMap.insert({ VK_LEFT, wd::Input::Key::Left });
	m_inputKeyMap.insert({ VK_RIGHT, wd::Input::Key::Right });
	m_inputKeyMap.insert({ VK_DOWN, wd::Input::Key::Down });
}

wd::Input::~Input(
	void)
{
	m_pInput = nullptr;
}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

bool wd::Input::IsKeyDown(
	const wd::Input::Key& key)
{
	return (static_cast<std::uint8_t>(key) & m_pInput->m_keysDown) != 0;
}

// Windows event appeared. Here we process it with our own event system, if needed.
LRESULT wd::Input::ProcessEvent(
	const HWND& hWindow,
	const UINT& wmCode,
	const WPARAM& wParam,
	const LPARAM& lParam)
{
	const UINT_PTR wParamInt = static_cast<UINT_PTR>(wParam);

	switch (wmCode)
	{
		case WM_KEYDOWN:
			WMKeyDown(wParamInt);
			break;

		case WM_KEYUP:
			WMKeyUp(wParamInt);
			break;

		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
	}
	return ::DefWindowProc(hWindow, wmCode, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Protected methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------

void wd::Input::WMKeyDown(
	const UINT_PTR& wParam)
{
	const auto it = m_inputKeyMap.find(wParam);
	if (it == m_inputKeyMap.end())
		return;

	if (it->second == wd::Input::Key::Back)
	{
		::PostQuitMessage(0);
		return;
	}

	if ((static_cast<std::uint8_t>(it->second) & m_keysDown) != 0)
		return;

	m_keysDown |= static_cast<std::uint8_t>(it->second);
}

void wd::Input::WMKeyUp(
	const UINT_PTR& wParam)
{
	const auto it = m_inputKeyMap.find(wParam);
	if (it == m_inputKeyMap.end())
		return;

	m_keysDown &= ~static_cast<std::uint8_t>(it->second);
}
