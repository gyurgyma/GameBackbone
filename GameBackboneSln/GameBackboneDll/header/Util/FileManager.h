#pragma once

#include <Util\DllUtil.h>
#include <Util\FileReader.h>

#include<string>

namespace GB {

	/// <summary>
	/// Used to hold File Readers and Writers as well as any file management functions such as:
	/// encryption, splitting a string, etc
	/// </summary>
	class libGameBackbone FileManager {
	public:
		FileManager();
		FileManager(const FileManager& manager) = delete;
		FileManager(FileManager&& manager) = delete;
		FileManager& operator= (const FileManager& manager) = delete;
		FileManager& operator= (FileManager&& manager) = delete;
		~FileManager();


		std::string encryptDecryptString(const std::string& stringToEncrypt, const std::string& key);
	private:

		// Store the FileReader and FileWriter as pointers so clients can inherit from them
		FileReader* reader;
	};
}