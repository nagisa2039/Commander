#include <DxLib.h>
#include "ImageLoader.h"
#include <cassert>
#include "Geometry.h"

ImageLoader::ImageLoader()
{
}


ImageLoader::~ImageLoader()
{
	for (auto& pair : _table)
	{
		DeleteGraph(pair.second);
	}
}

int ImageLoader::GetImageHandle(const char* path)
{
	if (_table.contains(path))
	{
		return _table[path];
	}
	else
	{
		int handle = LoadGraph(path);
		if (handle == -1)
		{
			assert(false);
			return handle;
		}
		else
		{
			_table.emplace(path, handle);
			return handle;
		}
	}
}

int ImageLoader::LoadMask(const char* path)
{
	if (_table.contains(path))
	{
		return _table[path];
	}
	else
	{
		int handle = DxLib::LoadMask(path);
		assert(handle != -1);
		_table[path] = handle;
		return handle;
	}
	return -1;
}

int ImageLoader::MakeScreen(const char* name, const Size& screenSize, bool alpha)
{
	if (!_table.contains(name))
	{
		_table[name] = DxLib::MakeScreen(screenSize.w, screenSize.h, alpha);
	}
	return _table[name];
}