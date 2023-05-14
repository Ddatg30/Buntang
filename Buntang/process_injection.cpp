#include "process_injection.h"

namespace win {

	DWORD ApcInjection::ApcInjectionWithDll(const PROCESS_INFORMATION& process_information, const std::wstring& dll_path){
		size_t dll_size = dll_path.size() * 2 + sizeof(wchar_t);

		LPVOID address = ::VirtualAllocEx(process_information.hProcess, nullptr, dll_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (!address){
			return ::GetLastError();
		}

		if (!::WriteProcessMemory(process_information.hProcess, address, dll_path.data(), dll_size, nullptr)){
			return ::GetLastError();
		}

		HMODULE module_handle = ::GetModuleHandleW(L"kernel32");
		if (module_handle){
			FARPROC func_pointer = ::GetProcAddress(module_handle, "LoadLibraryW");
			if (!::QueueUserAPC(reinterpret_cast<PAPCFUNC>(func_pointer), process_information.hThread, reinterpret_cast<ULONG_PTR>(address))){
				return ::GetLastError();
			}
		}

		//C28160, C6331 warning is inevitabe
		if (::VirtualFreeEx(process_information.hProcess, address, 0, MEM_RELEASE | MEM_DECOMMIT)){
			return ::GetLastError();
		}

		return ERROR_SUCCESS;
	}

} //namespace win