#pragma once
#include <unordered_map>
#include <string.h>
#include "Loader.h"

class SoundLoader :
    public Loader
{
private:
	std::unordered_map<std::string, int> _table;

public:
	bool Load(const char* path, Data& data);
	void Unload(const char* path);

	int GetSoundHandle(const char* path);

	bool PlayBGM(const char* path, const bool playTop = true);
	bool PlayBGM(const int handle, const bool playTop = true);
	bool PlaySE(const char* path);
	bool PlaySE(const int handle);
	bool StopSound();
};

