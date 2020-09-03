#include <DxLib.h>
#include "ImageLoader.h"



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

bool ImageLoader::Load(const char * path, Data & data)
{
	ImageData& img = dynamic_cast<ImageData&>(data);
	if (_table.contains(path))
	{
		img._handle = _table[path];
		return true;
	}
	else
	{
		img._handle = LoadGraph(path);
		if (img._handle == -1)
		{
			assert(false);
			return false;
		}
		else
		{
			_table.emplace(path, img._handle);
			return true;
		}
	}
}

int ImageLoader::MakeScreen(const char* name, const Size& screenSize, bool alpha)
{
	if (!_table.contains(name))
	{
		_table[name] = DxLib::MakeScreen(screenSize.w, screenSize.h, alpha);
	}
	return _table[name];
}

void ImageLoader::Unload(const char * path)
{
}
