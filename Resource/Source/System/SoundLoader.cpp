#include <Dxlib.h>
#include "SoundLoader.h"

bool SoundLoader::Load(const char* path, Data& data)
{
	return false;
}

void SoundLoader::Unload(const char* path)
{
}

int SoundLoader::GetSoundHandle(const char* path)
{
	if (_table.find(path) != _table.end())
	{
		return _table[path];
	}

	int handle = LoadSoundMem(path);
	assert(handle != -1);

	_table.emplace(path, handle);
	return handle;
}

bool SoundLoader::PlayBGM(const char* path, const bool playTop)
{
	return PlayBGM(GetSoundHandle(path), playTop);
}

bool SoundLoader::PlayBGM(const int handle, const bool playTop)
{
	return PlaySoundMem(handle, DX_PLAYTYPE_LOOP, playTop) != -1;
}

bool SoundLoader::PlaySE(const char* path)
{
	return PlaySE(GetSoundHandle(path));
}

bool SoundLoader::PlaySE(const int handle)
{
	return PlaySoundMem(handle, DX_PLAYTYPE_BACK, true) != -1;
}

bool SoundLoader::StopSound(const int handle)
{
	return StopSoundMem(handle) != -1;
}

bool SoundLoader::StopAllSound()
{
	for (const auto tableKey : _table)
	{
		StopSoundMem(tableKey.second);
	}
	return true;
}
