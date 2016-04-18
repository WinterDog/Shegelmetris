#include "inc/stdafx.h"

#include "Application/Application.h"

// Entry point to the program on Windows.
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
{
	// Suppress the warnings about unused variables.
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Create application object.
	wd::Application application;

	// Initialize primary objects like window, video and input devices, scene, etc.
	if (!application.Initialize(hInstance, nShowCmd))
		return 0;

	// Launch the main loop.
	const auto result = application.BeginGlobalLoop();

	// If we got here, the main loop ended. Return from the application.
	return result;
}
