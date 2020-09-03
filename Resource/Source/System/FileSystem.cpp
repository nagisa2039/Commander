#include "FileSystem.h"
#include "ImageLoader.h"
#include "FontLoader.h"
#include "SoundLoader.h"

using namespace std;

std::string GetExtension(const char* path)
{
	string str = path;
	auto idx = str.rfind('.')+1;
	return str.substr(idx, str.length() - idx);
}

FileSystem::FileSystem()
{
	_imageLoader = make_shared<ImageLoader>();
	_fontLoader = make_shared<FontLoader>();
	_soundLoder = make_shared<SoundLoader>();
}


FileSystem::~FileSystem()
{
}

int FileSystem::GetImageHandle(const char * path)
{
	ImageData imageData;
	_imageLoader->Load(path,imageData);
	return imageData.GetHandle();
}

int FileSystem::MakeScreen(const char* name, const Size& screenSize, const bool alpha)
{
	return _imageLoader->MakeScreen(name, screenSize, alpha);
}

bool FileSystem::FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic)
{
	return _fontLoader->FontInit(fontFile, fontName, useName, fontSize, fontThick, edgeFlag, italic);
}

int FileSystem::GetFontHandle(std::string fontUseName)
{
	return _fontLoader->GetFont(fontUseName);
}

int FileSystem::GetSoundHandle(const char* path)
{
	return _soundLoder->GetSoundHandle(path);
}

SoundLoader& FileSystem::GetSoundLoader()
{
	return *_soundLoder;
}

std::string FileSystem::GetFolderPass(std::string path)
{
	// imgFilePathÇÃçáê¨Ç∆ì«Ç›çûÇ›
	auto slash = path.find_last_of('/');
	return path.substr(0, slash + 1);
}

std::string FileSystem::GetFilePath(const std::string path, const bool reverse)
{
#ifdef _DEBUG
	if (reverse)
	{
		return "../" + path;
	}
	return path;
#else
	if (reverse)
	{
		return path;
	}
	auto slash = path.find_first_of('/');
	return path.substr(slash + 1);
#endif
}
