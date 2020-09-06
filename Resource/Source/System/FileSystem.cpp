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

	FontSetUp();
}


void FileSystem::FontSetUp()
{
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin20", 20, 1, false, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin30", 30, 1, false, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin40", 40, 1, false, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin50", 50, 1, false, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin60", 60, 1, false, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin70", 70, 1, false, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin80", 80, 1, false, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin100", 100, 1, false, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin200", 200, 1, false, false);

	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin20edge", 20, 1, true, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin30edge", 30, 1, true, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin40edge", 40, 1, true, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin50edge", 50, 1, true, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin60edge", 60, 1, true, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin70edge", 70, 1, true, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin80edge", 80, 1, true, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin100edge", 100, 1, true, false);
	FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin200edge", 200, 1, true, false);
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
