#pragma once
#include "Loader.h"
#include <map>
#include <string>
#include <windows.h>

class FontLoader :
	public Loader
{
private:
	std::map<LPCTSTR, int>		fontMap;		// ̫�ēo�^�pmap
	std::map<std::string, int> fontNameMap;	// ���O�t��map

public:
	FontLoader();
	~FontLoader();

	bool Load(const char* path, Data& data)override final;
	void Unload(const char* path)override final;

	// ̫�Ă̓ǂݍ��݁A�Q�ƂŕԂ�
	bool FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic);

	int SetFont(std::string useName);		// ���y�̾��(�������O)��ٰ�ߗp
};