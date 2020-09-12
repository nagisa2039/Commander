#include "AdvanceLoader.h"
#include "Application.h"
#include "DataBase.h"
#include "FileSystem.h"
#include "ImageLoader.h"
#include <DxLib.h>

AdvanceLoader::AdvanceLoader()
{
	auto getExt = [](const std::string& str)
	{
		auto lastSlash = str.rfind(".");
		return str.substr(lastSlash + 1, str.size() - lastSlash - 1);
	};

	_loadFuncMap["png"] = _loadFuncMap["jpg"] = [](const std::string& str) {ImageHandle(str.c_str());};
	_loadFuncMap["mp3"] = _loadFuncMap["wav"] = [](const std::string& str) {SoundHandle(str.c_str());};
	_loadFuncMap["bmp"] = [](const std::string& str) {ImageL.LoadMask(str.c_str()); };

	auto& advanceLoadTable = DataBase::Instance().GetAdvanceLoadTable();
	for (const auto& path : advanceLoadTable)
	{
		auto ext = getExt(path);
		if (_loadFuncMap.contains(ext))
		{
			_loadFuncMap[ext](path);
		}
		else
		{
			assert(false);
		}
	}
}

AdvanceLoader::~AdvanceLoader()
{
}
