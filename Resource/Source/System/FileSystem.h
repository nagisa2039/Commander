#pragma once
#include <memory>
#include<map>
#include "Loader.h"
#include <string>
#include <windows.h>

class SoundLoader;
class FontLoader;

class FileSystem
{
private:
	FileSystem(const FileSystem&) = delete;
	FileSystem& operator=(const FileSystem&) = delete;

	std::map<std::string,std::shared_ptr<Loader>>_loaders;
	std::shared_ptr<FontLoader> _fontLoader;
	std::shared_ptr<SoundLoader> _soundLoder;

public:
	FileSystem();
	~FileSystem();
	bool Load(const char* path, Data& data);

	int GetImageHandle(const char* path);

	bool FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic);
	int GetFontHandle(std::string fontUseName);

	int GetSoundHandle(const char* path);
	SoundLoader& GetSoundLoader();

	static std::string GetFolderPass(std::string);
	static std::string GetFilePath(const std::string path, const bool reverse = false);
};

 

