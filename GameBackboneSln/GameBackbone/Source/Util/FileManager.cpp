#include <GameBackbone/Core/BackboneBaseExceptions.h>
#include <GameBackbone/Util/FileManager.h>
#include <GameBackbone/Util/FileReader.h>

#include <exception>
#include <string>
#include <sys/stat.h>

using namespace GB;

FileManager::FileManager()
{
	reader = new FileReader;
}

FileManager::~FileManager()
{
	delete reader;
}

/// <summary>
/// Encrypts or decrypts a string.
/// </summary>
/// <param name="stringToChange">The string to be encrypted.</param>
/// <param name="key">The key for the encryption. Throws std::invalid_argument if the key is empty.</param>
/// <returns></returns>
std::string FileManager::encryptDecryptString(const std::string& stringToEncrypt, const std::string& key) const {
	if (key.empty()){
		throw std::invalid_argument("The key passed to FileManager::encryptDecryptString must not be empty.");
	}

	std::string output = stringToEncrypt;

	for (unsigned int ii = 0; ii < stringToEncrypt.size(); ++ii)
	{
		// XOR the char in stringToChange with the corresponding char from the key.
		output[ii] = stringToEncrypt[ii] ^ key[(ii % key.size())];
	}

	return output;
}

/// <summary>
/// Gets the size of the file. Return 0 if the file does not exist.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
std::size_t FileManager::getFileSize(const std::string& filename) const {
	struct stat st;
	if (stat(filename.c_str(), &st) != 0) {
		return 0;
	}
	return st.st_size;
}

/// <summary>
/// Gets a hash of the passed in file. This hash uses std::hash which only returns type std::size_t.
/// </summary>
/// <param name="filename">The filename.</param>
/// <returns></returns>
std::size_t FileManager::getFileHash(const std::string& filename) const {

	std::string file = reader->readFile(filename);
	return getHash(file);
}

/// <summary>
/// Gets a hash of the passed in string. This hash uses std::hash which only returns type std::size_t.
/// </summary>
/// <param name="toHash">The string to hash.</param>
/// <returns></returns>
size_t FileManager::getHash(const std::string& toHash) const {

	size_t hashed = std::hash<std::string>{}(toHash);
	return hashed;
}