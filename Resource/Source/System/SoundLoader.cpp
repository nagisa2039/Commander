#include <Dxlib.h>
#include <cassert>
#include "SoundLoader.h"

int SoundLoader::GetSoundHandle(const char* path)
{
	if (_table.contains(path))
	{
		return _table[path];
	}

	int handle = LoadSoundMem(path);
	assert(handle != -1);

	_table.emplace(path, handle);
	return handle;
}

bool SoundLoader::PlayBGM(const char* path, const int volume, const bool playTop)
{
	return PlayBGM(GetSoundHandle(path), volume, playTop);
}

bool SoundLoader::PlayBGM(const int handle, const int volume, const bool playTop)
{
	//çƒê∂íÜÇ»ÇÁâΩÇ‡ÇµÇ»Ç¢
	if (CheckSoundMem(handle) == 1)return false;
	ChangeVolumeSoundMem(volume, handle);
	return PlaySoundMem(handle, DX_PLAYTYPE_LOOP, playTop) != -1;
}

bool SoundLoader::PlaySE(const char* path, const int volume)
{
	return PlaySE(GetSoundHandle(path), volume);
}

bool SoundLoader::PlaySE(const int handle, const int volume)
{
	ChangeVolumeSoundMem(volume, handle);
	return PlaySoundMem(handle, DX_PLAYTYPE_BACK, true) != -1;
}

bool SoundLoader::StopSound(const int handle)
{
	int play = CheckSoundMem(handle);
	if (play == 1)
	{
		StopSoundMem(handle);
		return true;
	}
	return false;
}

bool SoundLoader::StopAllSound()
{
	for (const auto tableKey : _table)
	{
		StopSoundMem(tableKey.second);
	}
	return true;
}
