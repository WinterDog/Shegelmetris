#include "inc/stdafx.h"

#include "VideoDevice.h"

#pragma comment(lib, "opengl32.lib")

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------

const float wd::VideoDevice::m_cellOuterRadius = 0.5f;
const float wd::VideoDevice::m_cellInnerRadius = m_cellOuterRadius * 0.75f;

//-----------------------------------------------------------------------------
// Constructor and destructor
//-----------------------------------------------------------------------------

wd::VideoDevice::VideoDevice(
	void)
	:
	m_hDC(nullptr),
	m_hGLRC(nullptr)
{
}

wd::VideoDevice::~VideoDevice(
	void)
{
	// Shutdown all OpenGL stuff.
	Shutdown(m_hWnd);
}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

bool wd::VideoDevice::Initialize(
	const HWND& hWnd,
	const HDC& hDC,
	const glm::u32vec2& screenSize,
	const glm::vec2& fieldSize,
	const float& marginLeft,
	const float& marginTop,
	const float& marginBottom)
{
	// Save input to the members.
	m_hWnd = hWnd;
	m_hDC = hDC;
	m_aspectRatio = screenSize.x / static_cast<float>(screenSize.y);

	// Init PFD.
	PIXELFORMATDESCRIPTOR formatDesc;
	ZeroMemory(&formatDesc, sizeof(formatDesc));

	formatDesc.nSize = sizeof(formatDesc);
	formatDesc.nVersion = 1;
	formatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
	formatDesc.iPixelType = PFD_TYPE_RGBA;
	formatDesc.cColorBits = 24;
	formatDesc.cDepthBits = 16;
	formatDesc.iLayerType = PFD_MAIN_PLANE;

	{
		// Choose appropriate PF by the description
		const int format = ::ChoosePixelFormat(m_hDC, &formatDesc);
		const auto success = ::SetPixelFormat(m_hDC, format, &formatDesc);
		if (!success)
			assert(false);
	}

	// Create and enable the render context.
	m_hGLRC = ::wglCreateContext(m_hDC);
	const auto success = ::wglMakeCurrent(m_hDC, m_hGLRC);
	if (!success)
	{
		assert(false);
		return false;
	}

	// Init OpenGL stuff.
	::glViewport(0, 0, screenSize.x, screenSize.y);
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	// Setup orthographic projection matrix.
	m_viewSize.y = fieldSize.y + marginTop + marginBottom;
	m_viewSize.x = m_viewSize.y * m_aspectRatio;

	::glOrtho(
		-m_viewSize.x / 2.0f + fieldSize.x / 2.0f,
		m_viewSize.x / 2.0f + fieldSize.x / 2.0f,
		-marginBottom,
		m_viewSize.y - marginBottom,
		0.1f,
		10.0f);

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();

	// Create font.
	BuildFont();

	return true;
}

void wd::VideoDevice::ClearBuffer(
	const glm::vec4& color)
{
	// Set the color to clear the screen to.
	::glClearColor(color.r, color.g, color.b, color.a);
	// Clear the screen and depth buffer.
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Move camera back a bit so our stuff would be visible.
	::glLoadIdentity();
	::glTranslatef(0.0f, 0.0f, -1.0f);
}

void wd::VideoDevice::RenderText(
	const std::string& text,
	const glm::vec2& position,
	const glm::vec3& color)
{
	// Draw text.
	::glColor3f(color.x, color.y, color.z);
	::glRasterPos2f(m_viewSize.x / 2.0f * position.x, m_viewSize.y * position.y);

	::glPushAttrib(GL_LIST_BIT);
	::glListBase(m_fontBase);
	::glCallLists(static_cast<GLsizei>(text.size()), GL_UNSIGNED_BYTE, text.c_str());
	::glPopAttrib();
}

void wd::VideoDevice::RenderCell(
	const glm::vec2& position,
	const glm::vec3& color)
{
	// Lighter color for the top-left part of the cell.
	const glm::vec3 colorLighter = glm::saturate(color * 1.2f);
	// Darker color for the bottom-right part of the cell.
	// These 2 will allow to create "lighting" effect.
	const glm::vec3 colorDarker = color * 0.8f;

	// Render top-left part of the cell.
	::glBegin(GL_TRIANGLES);
	::glColor3f(colorLighter.r, colorLighter.g, colorLighter.b);
	::glVertex2f(position.x - m_cellOuterRadius, (position.y - m_cellOuterRadius));
	::glVertex2f(position.x - m_cellOuterRadius, (position.y + m_cellOuterRadius));
	::glVertex2f(position.x + m_cellOuterRadius, (position.y + m_cellOuterRadius));
	::glEnd();

	// Render bottom-down part of the cell.
	::glBegin(GL_TRIANGLES);
	::glColor3f(colorDarker.r, colorDarker.g, colorDarker.b);
	::glVertex2f(position.x - m_cellOuterRadius, (position.y - m_cellOuterRadius));
	::glVertex2f(position.x + m_cellOuterRadius, (position.y - m_cellOuterRadius));
	::glVertex2f(position.x + m_cellOuterRadius, (position.y + m_cellOuterRadius));
	::glEnd();

	// Render center (smaller) part of the cell.
	::glBegin(GL_QUADS);
	::glColor3f(color.r, color.g, color.b);
	::glVertex2f(position.x - m_cellInnerRadius, (position.y - m_cellInnerRadius));
	::glVertex2f(position.x + m_cellInnerRadius, (position.y - m_cellInnerRadius));
	::glVertex2f(position.x + m_cellInnerRadius, (position.y + m_cellInnerRadius));
	::glVertex2f(position.x - m_cellInnerRadius, (position.y + m_cellInnerRadius));
	::glEnd();
}

void wd::VideoDevice::PresentFrame(
	void)
{
	// Present the back buffer to the screen since rendering is complete.
	::SwapBuffers(m_hDC);
}

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------

void wd::VideoDevice::BuildFont(
	void)
{
	// OpenGL object which will contain the font data.
	m_fontBase = ::glGenLists(256);

	// Create new font object with our settings.
	HFONT font = ::CreateFont(
		-16,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		// For Russian symbols to appear, we need to set it to default.
		DEFAULT_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		"Courier New");

	// Select the current default device context font.
	HFONT oldFont = (HFONT)::SelectObject(m_hDC, font);

	{
		// Create OpenGL object containing the font data.
		const auto success = ::wglUseFontBitmaps(m_hDC, 0, 256, m_fontBase);
		if (!success)
		{
			// Some error at the font creation.
			const auto result = ::GetLastError();
			assert(false);
		}
	}
	// Select old font as the default device context font.
	::SelectObject(m_hDC, oldFont);

	// Now we can delete the font - OpenGL list was already created.
	{
		const auto success = ::DeleteObject(font);
		if (!success)
			assert(false);
	}
}

void wd::VideoDevice::Shutdown(
	const HWND& hWnd)
{
	// Free OpenGL stuff.
	::glDeleteLists(m_fontBase, 256);
	// Reset current context.
	::wglMakeCurrent(nullptr, nullptr);
	// Delete OpenGL rendering context.
	::wglDeleteContext(m_hGLRC);
	::ReleaseDC(hWnd, m_hDC);
}
