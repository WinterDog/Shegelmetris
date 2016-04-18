#include "inc/stdafx.h"

#include "String.h"

//#include <locale>
#include <codecvt>

std::wstring wd::StringToWide(
	const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::wstring wd::StringToWide(
	const char* const pChar)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

	return converterX.from_bytes(pChar);
}

std::string wd::WideToString(
	const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::string wd::WideToString(
	const wchar_t* const pChar)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

	return converterX.to_bytes(pChar);
}
