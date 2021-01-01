#pragma once
#include <map>
#include <string>
#include <windows.h>

/// <summary>
/// �t�H���g�Ǎ��N���X
/// </summary>
class FontLoader
{
private:
	// ̫�ēo�^�pmap
	std::map<LPCTSTR, int>		fontMap;	
	// ���O�t��map
	std::map<std::string, int> fontNameMap;

public:
	FontLoader();
	~FontLoader();

	/// <summary>
	/// �t�H���g�̏�����
	/// </summary>
	/// <param name="fontFile">�t�H���g�t�@�C���p�X</param>
	/// <param name="fontName">�t�H���g��</param>
	/// <param name="useName">�}�b�v�ɓo�^���g�p����ۂ̃t�H���g��</param>
	/// <param name="fontSize">�t�H���g�T�C�Y</param>
	/// <param name="fontThick">����</param>
	/// <param name="edgeFlag">�������邩</param>
	/// <param name="italic">�C�^���b�N�̂ɂ��邩</param>
	/// <returns>����</returns>
	bool FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic);

	/// <summary>
	/// �t�H���g�n���h���̎擾
	/// </summary>
	/// <param name="useName">�t�H���g��</param>
	int GetFont(std::string useName);
};