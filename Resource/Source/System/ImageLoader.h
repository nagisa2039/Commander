#pragma once
#include <map>
#include <string>

struct Size;

/// <summary>
/// �摜�Ǎ��N���X
/// </summary>
class ImageLoader
{
private:
	// ���\�[�X�Ǘ��z��
	std::map<std::string, int> _table;
public:
	ImageLoader();
	~ImageLoader();

	/// <summary>
	/// �摜�n���h���̎擾
	/// </summary>
	/// <param name="path">�摜�n���h���̎擾</param>
	int GetImageHandle(const char* path);

	/// <summary>
	/// �}�X�N�摜�̓ǂݍ���
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	int LoadMask(const char* path);

	/// <summary>
	/// �X�N���[���̐���
	/// </summary>
	/// <param name="name">�X�N���[����</param>
	/// <param name="screenSize">�X�N���[���T�C�Y</param>
	/// <param name="alpha">�����x���g����</param>
	int MakeScreen(const char* name, const Size& screenSize, bool alpha = false);
};