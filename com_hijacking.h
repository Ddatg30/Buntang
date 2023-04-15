#pragma once

#include <Windows.h>

#include <string>
#include <vector>

//reg add "HKCU\SOFTWARE\Classes\CLSID\{1299CF18-C4F5-4B6A-BB0F-2299F0398E27}\InprocServer32" / ve / t REG_SZ / d "hello" / f
//reg add "HKCU\SOFTWARE\Classes\CLSID\{1299CF18-C4F5-4B6A-BB0F-2299F0398E27}\InprocServer32" / v "ThreadingModel" / t REG_SZ / d "Apartment" / f
//reg add "HKCR\SOFTWARE\Classes\CLSID\{1299CF18-C4F5-4B6A-BB0F-2299F0398E27}\Elevation" /ve /t REG_SZ /d "hello"  /f


namespace win {

	DWORD GetTargetCLSIDList(std::vector<std::wstring>* clsid_list){

		std::vector<std::wstring> qp;

		*clsid_list = {L"HKCU\SOFTWARE\Classes\CLSID\{1299CF18-C4F5-4B6A-BB0F-2299F0398E27}"};

		return ERROR_SUCCESS;
	}


} //namespace win
