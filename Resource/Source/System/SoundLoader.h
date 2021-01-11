#pragma once
#include <unordered_map>
#include <string.h>

/// <summary>
/// ���Ǎ��N���X
/// </summary>
class SoundLoader
{
private:
	// �n���h���Ǘ��}�b�v<�t�@�C���p�X, �n���h��>
	std::unordered_map<std::string, int> _handleTable;

	// �n���h�����
	struct HandleInf
	{
		// �{�����[��
		int volume = 255;
		// �Đ���
		bool play = false;
	};

	// ���ʊǗ��}�b�v<�n���h��, >
	std::unordered_map<int, HandleInf> _handleInfTable;
	// �}�X�^�[�{�����[��
	float _masterVolume;

	/// <summary>
	/// ���ʂ̕ύX
	/// </summary>
	/// <param name="handle">���n���h��</param>
	/// <param name="volume">����</param>
	void ChangeVolume(const int handle, const int volume);

public:
	SoundLoader();
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

	/// <summary>
	/// �}�X�^�[�{�����[���̕ύX(0�`255)
	/// </summary>
	void ChangeMasterVolume(const int volume);

	/// <summary>
	/// �n���h���̍Đ��󋵂��擾
	/// </summary>
	/// <param name="handle">���n���h��</param>
	/// <returns>�Đ����Ă邩</returns>
	bool PlayCheck(const int handle);
};

