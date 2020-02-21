#pragma once
#include <memory>
#include<map>
#include "Loader.h"
#include <string>

class FileSystem
{
private:
	std::map<std::string,std::shared_ptr<Loader>>_loaders;

public:
	FileSystem();
	~FileSystem();
	bool Load(const char* path, Data& data);

	int GetImageHandle(const char* path);


	static std::string GetFolderPass(std::string);
	static std::string GetFilePath(const std::string path, const bool reverse = false);
};

 

