#include <DxLib.h>
#include "ImageLoader.h"



ImageLoader::ImageLoader()
{
}


ImageLoader::~ImageLoader()
{
}

bool ImageLoader::Load(const char * path, Data & data)
{
	ImageData& img = dynamic_cast<ImageData&>(data);
	auto it = _table.find(path);
	if (it == _table.end())
	{
		img._handle = LoadGraph(path);
		if (img._handle == -1)
		{
			//assert(false);
			return false;
		}
		else
		{
			_table.emplace(path,img._handle);
			return true;
		}
	}
	else
	{
		img._handle = _table[path];
		return true;
	}
}

void ImageLoader::Unload(const char * path)
{
}
