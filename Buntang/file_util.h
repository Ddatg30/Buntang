#pragma once

#include <Windows.h>

#include <vector>
#include <string>

namespace win {

	namespace util {
		DWORD CreateDataFile(HANDLE* file_handle, const std::wstring& file_path);
		DWORD OpenDataFile(HANDLE* file_handle, const std::wstring& file_path);
		DWORD ReadData(HANDLE file_handle, LPVOID* buffer_ptr, DWORD* buffer_size);
		DWORD WriteData(HANDLE file_handle, const std::wstring& buffer, DWORD buffer_length);

	} //namespace util

} //namespace win

