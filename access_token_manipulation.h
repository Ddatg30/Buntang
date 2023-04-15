#pragma once

#include <Windows.h>
#include <string>

namespace win {


	class AccessTokenManipulation {
	public:
		DWORD ParentPidSpoofing(DWORD parent_proc_infod, const std::wstring& process_path);
		DWORD TokenImpersonationTheft(DWORD target_pid, const std::wstring& create_process_name);
	};

} //namespace win