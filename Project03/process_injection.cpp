#include "process_injection.h"

namespace win {

	DWORD ApcInjection::ApcInjectionWithDll(const PROCESS_INFORMATION& process_information, std::wstring_view dll_path)
	{
		size_t dll_size = dll_path.size() * 2 + sizeof(wchar_t);

		LPVOID address = ::VirtualAllocEx(process_information.hProcess, nullptr, dll_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (!address)
		{
			return ::GetLastError();
		}

		if (!::WriteProcessMemory(process_information.hProcess, address, dll_path.data(), dll_size, nullptr))
		{
			return ::GetLastError();
		}

		if (!::QueueUserAPC((PAPCFUNC)::GetProcAddress(GetModuleHandleW(L"kernel32"), "LoadLibraryW"), process_information.hThread, (ULONG_PTR)address))
		{
			return ::GetLastError();
		}

		if (::VirtualFreeEx(process_information.hProcess, address, 0, MEM_RELEASE | MEM_DECOMMIT))
		{
			return ::GetLastError();
		}

		return ERROR_SUCCESS;
	}


} //namespace win