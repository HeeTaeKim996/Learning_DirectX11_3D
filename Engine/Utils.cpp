#include "pch.h"
#include "Utils.h"

#if 1
bool Utils::StartsWith(string str, string comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

bool Utils::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

std::wstring Utils::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

std::string Utils::ToString(wstring value)
{
	return string(value.begin(), value.end());
}

void Utils::Replace(OUT string& str, string comp, string rep)
{
	string temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != string::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

void Utils::Replace(OUT wstring& str, wstring comp, wstring rep)
{
	wstring temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}





#endif













































#if 0 // First
bool Utils::StartsWith(string str, string comp)
{
	string::size_type index = str.find(comp);
	if (index != string::npos && index == 0)
	{
		return true;
	}

	return false;
}

bool Utils::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
	{
		return true;
	}

	return false;
}

std::wstring Utils::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

std::string Utils::ToString(wstring value)
{
	return string(value.begin(), value.end());
}

#endif // First