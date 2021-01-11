#include <Dxlib.h>
#include <cassert>
#include "SoundLoader.h"
#include "Cast.h"


SoundLoader::SoundLoader()
{
	_masterVolume = 0.0f;
}

int SoundLoader::GetSoundHandle(const char* path)
{
	if (_handleTable.contains(path))
	{
		return _handleTable[path];
	}

	int handle = LoadSoundMem(path);
	assert(handle != -1);

	_handleTable.emplace(path, handle);
	_handleInfTable.emplace(handle, HandleInf{ 255, false });
	return handle;
}

bool SoundLoader::PlayBGM(const char* path, const int volume, const bool playTop)
{
	return PlayBGM(GetSoundHandle(path), volume, playTop);
}

bool SoundLoader::PlayBGM(const int handle, const int volume, const bool playTop)
{
	//çƒê∂íÜÇ»ÇÁâΩÇ‡ÇµÇ»Ç¢
	if (PlayCheck(handle))return false;
	ChangeVolume(handle, volume);
	return PlaySoundMem(handle, DX_PLAYTYPE_LOOP, playTop) != -1;
}

bool SoundLoader::PlaySE(const char* path, const int volume)
{
	return PlaySE(GetSoundHandle(path), volume);
}

bool SoundLoader::PlaySE(const int handle, const int volume)
{
	PlayCheck(handle);
	ChangeVolume(handle, volume);
	return PlaySoundMem(handle, DX_PLAYTYPE_BACK, true) != -1;
}

void SoundLoader::ChangeVolume(const int handle, const int volume)
{
	_handleInfTable[handle].volume = volume;
	ChangeVolumeSoundMem(Int32(volume * _masterVolume), handle);
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
	for (const auto tableKey : _handleTable)
	{
		StopSoundMem(tableKey.second);
	}
	return true;
}

void SoundLoader::ChangeMasterVolume(const int volume)
{
	_masterVolume = volume / 255.0f;
	for (const auto tableKey : _handleInfTable)
	{
		if (PlayCheck(tableKey.first))
		{
			ChangeVolume(tableKey.first, tableKey.second.volume);
		}
	}
}

bool SoundLoader::PlayCheck(const int handle)
{
	if (!_handleInfTable[handle].play)
	{
		return false;
	}
	bool play = CheckSoundMem(handle) == 1;
	_handleInfTable[handle].play = play;
	return play;
}
