#include "FileSystem.h"
#include "ImageLoader.h"
#include "FontLoader.h"

using namespace std;

std::string GetExtension(const char* path)
{
	string str = path;
	auto idx = str.rfind('.')+1;
	return str.substr(idx, str.length() - idx);
}

FileSystem::FileSystem()
{
	auto imgLoader = make_shared<ImageLoader>();
	_loaders["bmp"] = _loaders["png"] = _loaders["jpg"] = imgLoader;

	_fontLoader = make_shared<FontLoader>();
}


FileSystem::~FileSystem()
{
}

bool FileSystem::Load(const char * path, Data & data)
{
	//auto convertPath = GetFilePath(path);
	auto ext = GetExtension(path);
	return _loaders[ext]->Load(path,data);
}

int FileSystem::GetImageHandle(const char * path)
{
	ImageData imageData;
	Load(path,imageData);
	return imageData.GetHandle();
}

bool FileSystem::FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic)
{
	return _fontLoader->FontInit(fontFile, fontName, useName, fontSize, fontThick, edgeFlag, italic);
}

int FileSystem::GetFontHandle(std::string fontUseName)
{
	return _fontLoader->SetFont(fontUseName);
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
