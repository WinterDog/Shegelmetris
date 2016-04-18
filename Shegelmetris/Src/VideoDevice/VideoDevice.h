#pragma once

// Include Windows stuff.
#include <windows.h>
// Include OpenGL.
#include <gl/GL.h>

namespace wd
{
	//-----------------------------------------------------------------------------
	// OpenGL device, context and renderer wrapper.
	//-----------------------------------------------------------------------------
	class VideoDevice
	{
	private:
		//-----------------------------------------------------------------------------
		// Static members
		//-----------------------------------------------------------------------------

		// Outer radius of the cell.
		static const float						m_cellOuterRadius;
		// Inner radius of the cell (to create 3D illusion).
		static const float						m_cellInnerRadius;

		//-----------------------------------------------------------------------------
		// Non-static members
		//-----------------------------------------------------------------------------

		// Size of the view area.
		glm::vec2								m_viewSize;
		// Screen aspect ratio.
		float									m_aspectRatio;

		// Window handler.
		HWND									m_hWnd;
		// Weak pointer from the Window.
		HDC										m_hDC;
		// Rendering context.
		HGLRC									m_hGLRC;
		// Base display list for the font set.
		GLuint									m_fontBase;

	public:
		//-----------------------------------------------------------------------------
		// Constructor and destructor
		//-----------------------------------------------------------------------------

		VideoDevice(
			void);

		~VideoDevice(
			void);

	public:
		//-----------------------------------------------------------------------------
		// Methods
		//-----------------------------------------------------------------------------

		bool Initialize(
			const HWND& hWnd,
			const HDC& hDC,
			const glm::u32vec2& screenSize,
			const glm::vec2& fieldSize,
			const float& marginLeft,
			const float& marginTop,
			const float& marginBottom);

		// Clear buffer.
		void ClearBuffer(
			const glm::vec4& color);

		// Render string.
		void RenderText(
			const std::string& text,
			const glm::vec2& position,
			const glm::vec3& color);

		// Render cell.
		void RenderCell(
			const glm::vec2& position,
			const glm::vec3& color);

		// Present new frame (swap buffers basically).
		void PresentFrame(
			void);

	private:
		// Build font for text rendering.
		void BuildFont(
			void);

		// Shutdown OpenGL stuff.
		void Shutdown(
			const HWND& hwnd);
	};
};
