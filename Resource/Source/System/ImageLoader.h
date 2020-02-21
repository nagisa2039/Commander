#pragma once
#include <map>
#include "Loader.h"
#include <string>

class ImageLoader :
	public Loader
{
private:
	std::map<std::string, int> _table;
public:
	ImageLoader();
	~ImageLoader();
	bool Load(const char* path, Data& data)override; 
	void Unload(const char* path)override;
};

class ImageData : public Data
{
	friend ImageLoader;
private:
	int _handle;
public:
	const int GetHandle()const { return _handle; }
	bool IsAvailable() { return _handle != -1; }
};

