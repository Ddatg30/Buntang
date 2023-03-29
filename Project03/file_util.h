#pragma once

#include <Windows.h>

#include <string>

namespace win {

	namespace util {
		DWORD CreateDataFile(HANDLE* file_handle, std::wstring_view file_path);
		DWORD OpenDataFile(HANDLE* file_handle, std::wstring_view file_path);
		DWORD ReadData(HANDLE file_handle, LPVOID* buffer_ptr, DWORD* buffer_size);
		DWORD WriteData(HANDLE file_handle, std::wstring_view buffer, DWORD buffer_length);


	
	} //namespace util



} //namespace win

