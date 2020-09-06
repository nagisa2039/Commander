#pragma once
#include <memory>
#include<map>
#include <string>
#include <windows.h>

class SoundLoader;
class FontLoader;
class ImageLoader;

struct Size;

class FileSystem
{
private:
	FileSystem();
	FileSystem(const FileSystem&) = delete;
	FileSystem& operator=(const FileSystem&) = delete;

	std::shared_ptr<ImageLoader> _imageLoader;
	std::shared_ptr<FontLoader> _fontLoader;
	std::shared_ptr<SoundLoader> _soundLoder;

	void FontSetUp();

public:
	~FileSystem();

	// ƒVƒ“ƒOƒ‹ƒgƒ“ŽÀ‘Ì‚ð•Ô‚·
	static FileSystem& Instance()
	{
		static FileSystem instance;
		return instance;
	}

	int GetImageHandle(const char* path);
	int MakeScreen(const char* name, const Size& screenSize, const bool alpha = false);

	bool FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic);
	int GetFontHandle(std::string fontUseName);

	int GetSoundHandle(const char* path);
	SoundLoader& GetSoundLoader();

	static std::string GetFolderPass(std::string);
	static std::string GetFilePath(const std::string path, const bool reverse = false);
};

#define ImageHandle(X) FileSystem::Instance().GetImageHandle(X)
#define SoundHandle(X) FileSystem::Instance().GetSoundHandle(X)
#define FontHandle(X)  FileSystem::Instance().GetFontHandle(X)

#define SoundL	FileSystem::Instance().GetSoundLoader()