#pragma once

#include <Windows.h>
#include <TlHelp32.h>

#include <vector>
#include <string>

namespace win {

	namespace util {
		DWORD GetPidList(const std::wstring process_name, std::vector<DWORD>* pid_list) {
			std::vector<DWORD> local_pid_list;
			HANDLE snap_shot_handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD, 0);
			if (snap_shot_handle == INVALID_HANDLE_VALUE) {
				return ::GetLastError();
			}
			PROCESSENTRY32W process_entry = { sizeof(process_entry) };
			::Process32FirstW(snap_shot_handle, &process_entry);

			do {
				if (process_name == process_entry.szExeFile) {
					local_pid_list.push_back(process_entry.th32ProcessID);
				}
			} while (::Process32NextW(snap_shot_handle, &process_entry));
			::CloseHandle(snap_shot_handle);

			*pid_list = local_pid_list;

			return ERROR_SUCCESS;
		}

	} //namespace util

} //namespace win

