#pragma once
#include <filesystem>

namespace UserData
{

bool Save(const std::filesystem::path& filename, const char* src, std::streamsize size);

template <typename T>
bool Save(const std::filesystem::path& filename, const T& obj)
{
	return Save(filename, reinterpret_cast<const char*>(&obj), sizeof(T));
}

bool Load(const std::filesystem::path& filename, char* dest, std::streamsize size);

template <typename T>
bool Load(const std::filesystem::path& filename, T& obj)
{
	return Load(filename, reinterpret_cast<char*>(&obj), sizeof(T));
}

}
