#pragma once

namespace wd
{
	std::wstring StringToWide(
		const std::string& str);

	std::wstring StringToWide(
		const char* const pChar);

	std::string WideToString(
		const std::wstring& str);

	std::string WideToString(
		const wchar_t* const pChar);
};
