#pragma once
#include <memory>
#include<map>
#include <string>
#include <windows.h>

class SoundLoader;
class FontLoader;
class ImageLoader;

struct Size;

/// <summary>
/// ���\�[�X�t�@�C���Ǘ��N���X
/// </summary>
class FileSystem
{
private:
	FileSystem();
	FileSystem(const FileSystem&) = delete;
	FileSystem& operator=(const FileSystem&) = delete;

	// �摜�Ǎ��N���X
	std::shared_ptr<ImageLoader> _imageLoader;
	// �t�H���g�Ǎ��N���X
	std::shared_ptr<FontLoader> _fontLoader;
	// ���Ǎ��N���X
	std::shared_ptr<SoundLoader> _soundLoder;

	/// <summary>
	/// �t�H���g�̃Z�b�g�A�b�v
	/// </summary>
	void FontSetUp();

public:
	~FileSystem();

	/// <summary>
	/// �V���O���g�����̂�Ԃ�
	/// </summary>
	static FileSystem& Instance()
	{
		static FileSystem instance;
		return instance;
	}

	/// <summary>
	/// �摜�n���h���̎擾
	/// </summary>
	/// <param name="path">�摜�t�@�C���p�X</param>
	int GetImageHandle(const char* path);

	/// <summary>
	/// �X�N���[���𐶐����n���h����Ԃ�
	/// </summary>
	/// <param name="name">�X�N���[����</param>
	/// <param name="screenSize">�X�N���[���T�C�Y</param>
	/// <param name="alpha">�����x���g����</param>
	int MakeScreen(const char* name, const Size& screenSize, const bool alpha = false);

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
	/// <param name="fontUseName">�t�H���g��</param>
	int GetFontHandle(std::string fontUseName);

	/// <summary>
	/// ���n���h���̎擾
	/// </summary>
	/// <param name="path">���t�@�C���p�X</param>
	int GetSoundHandle(const char* path);

	/// <summary>
	/// �摜�Ǎ��N���X�̎擾
	/// </summary>
	ImageLoader& GetImageLoader();

	/// <summary>
	/// ���n���h���̎擾
	/// </summary>
	SoundLoader& GetSoundLoader();

	/// <summary>
	/// �t�H���g�n���h���̎擾
	/// </summary>
	FontLoader& GetFontLoader();

	/// <summary>
	/// �t�@�C���p�X����t�H���_�p�X�𒊏o����
	/// </summary>
	/// <param name="">�t�@�C���p�X</param>
	static std::string GetFolderPass(std::string path);
};

// �摜�n���h���擾
#define ImageHandle(X) FileSystem::Instance().GetImageHandle(X)
// ���n���h���擾
#define SoundHandle(X) FileSystem::Instance().GetSoundHandle(X)
// �t�H���g�n���h���擾
#define FontHandle(X)  FileSystem::Instance().GetFontHandle(X)

// �摜�Ǎ��N���X�擾
#define ImageL	FileSystem::Instance().GetImageLoader()
// ���Ǎ��N���X�擾
#define SoundL	FileSystem::Instance().GetSoundLoader()
// �t�H���g�Ǎ��N���X�擾
#define FontL	FileSystem::Instance().GetFontLoader()