#pragma once
#include <unordered_map>
#include <string.h>

/// <summary>
/// ���Ǎ��N���X
/// </summary>
class SoundLoader
{
private:
	// ���\�[�X�Ǘ��z��
	std::unordered_map<std::string, int> _table;

public:
	SoundLoader()=default;
	~SoundLoader()=default;

	/// <summary>
	/// ���n���h���̎擾
	/// </summary>
	/// <param name="path">���t�@�C���p�X</param>
	int GetSoundHandle(const char* path);

	/// <summary>
	/// BGM�̍Đ�
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <param name="volume">����</param>
	/// <param name="playTop">�擪����Đ�</param>
	bool PlayBGM(const char* path, const int volume = 255, const bool playTop = true);

	/// <summary>
	/// BGM�̍Đ�
	/// </summary>
	/// <param name="handle">���n���h��</param>
	/// <param name="volume">����</param>
	/// <param name="playTop">�擪����Đ�</param>
	bool PlayBGM(const int handle, const int volume = 255, const bool playTop = true);

	/// <summary>
	/// SE�̍Đ�
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <param name="volume">����</param>
	/// <returns></returns>
	bool PlaySE(const char* path, const int volume = 255);

	/// <summary>
	/// SE�̍Đ�
	/// </summary>
	/// <param name="handle">���n���h��</param>
	/// <param name="volume">����</param>
	/// <returns></returns>
	bool PlaySE(const int handle, const int volume = 255);

	/// <summary>
	/// �����~
	/// </summary>
	/// <param name="handle">���n���h��</param>
	bool StopSound(const int handle);

	/// <summary>
	/// �S�Ẳ����~
	/// </summary>
	bool StopAllSound();
};

