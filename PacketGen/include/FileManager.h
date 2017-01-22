#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <fstream>

class FileManager
{
public:
		static bool IsDirectory(const std::string& path)
		{
				struct stat info;
				stat(path.c_str(), &info);
				return (info.st_mode & S_IFDIR);
		}

		static bool IsFile(const std::string& path)
		{
				std::ifstream fileIn(path.c_str());
				return fileIn.good();
		}
};

