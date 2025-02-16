#include "UserData.h"
#include <fstream>

namespace UserData
{

static const std::filesystem::path UserDataDirectory = "usr";

bool Save(const std::filesystem::path& filename, const char* src, std::streamsize size)
{
	auto fullPath = UserDataDirectory / filename;
	auto directory = fullPath;
	directory.remove_filename();
	if (!std::filesystem::exists(directory))
	{
		std::filesystem::create_directories(directory);
	}

	std::ofstream file(fullPath, std::ios::binary);
	if (!file)
		return false;

	file.write(src, size);
	return true;
}

bool Load(const std::filesystem::path& filename, char* dest, std::streamsize size)
{
	auto fullPath = UserDataDirectory / filename;
	std::ifstream file(fullPath, std::ios::binary);

	if (!file)
		return false;

	file.read(dest, size);
	return file.gcount() == size;
}

}
