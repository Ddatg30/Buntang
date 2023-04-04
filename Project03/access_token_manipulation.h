#pragma once

#include <Windows.h>
#include <string>

namespace win {


	class DataEncryptedforImpact {
	public:
		DWORD ParentPidSpoofing(DWORD parent_proc_infod, std::wstring_view process_path);
		DWORD TokenImpersonationTheft(DWORD target_pid, std::wstring_view create_process_name);
	};

} //namespace win