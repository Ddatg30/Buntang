#include "file_util.h"

namespace win {
	namespace util {

		DWORD CreateDataFile(HANDLE* file_handle, std::wstring_view file_path) {
			*file_handle = ::CreateFileW(file_path.data(),
				GENERIC_WRITE,
				0,
				nullptr,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				nullptr);
			if (*file_handle == INVALID_HANDLE_VALUE) {
				return ::GetLastError();
			}

			return ERROR_SUCCESS;
		}

		//Exist file
		DWORD OpenDataFile(HANDLE* file_handle, std::wstring_view file_path) {
			*file_handle = ::CreateFileW(file_path.data(),
				FILE_READ_DATA,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				nullptr,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				nullptr);
			if (*file_handle == INVALID_HANDLE_VALUE) {
				return ::GetLastError();
			}

			return ERROR_SUCCESS;
		}

		DWORD ReadData(HANDLE file_handle, LPVOID* buffer_ptr, DWORD* buffer_size) {
			DWORD bytes_read = 0;
			DWORD fileSize = ::GetFileSize(file_handle, nullptr);

			LPVOID buffer = ::VirtualAlloc(nullptr, fileSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (buffer == nullptr) {
				::CloseHandle(file_handle);
				return ::GetLastError();
			}
			else {
				if (!::ReadFile(file_handle, buffer, fileSize, &bytes_read, nullptr)) {
					::VirtualFree(buffer, 0, MEM_RELEASE);
					::CloseHandle(file_handle);
					return ::GetLastError();
				}
			}

			*buffer_size = fileSize;
			*buffer_ptr = buffer;

			return ERROR_SUCCESS;
		}

		DWORD WriteData(HANDLE file_handle, std::wstring_view buffer, DWORD buffer_length) {
			DWORD bytesWritten;
			if (!::WriteFile(file_handle, buffer.data(), buffer_length, &bytesWritten, NULL)) {
				::CloseHandle(file_handle);
				return GetLastError();
			}

			return ERROR_SUCCESS;
		}
	} //namespace util

} //namespace win

