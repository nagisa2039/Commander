#pragma once
#include <memory>
#include "TimeLine.h"

/// <summary>
/// �V�[���J�ڎ��̃t�F�[�h�Đ��N���X
/// </summary>
class Fade
{
private:
	// �A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _fadeTrack;
	// �t�F�[�h���̐F
	unsigned int _fadeColor;

public:
	Fade();
	~Fade();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �t�F�[�h�A�E�g���J�n����
	/// </summary>
	/// <returns></returns>
	bool GetEnd()const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="color">�t�F�[�h���̐F</param>
	void StartFadeIn(const unsigned color = 0x000000);

	/// <summary>
	/// �t�F�[�h�C�����J�n����
	/// </summary>
	/// <param name="color">�t�F�[�h���̐F</param>
	void StartFadeOut(const unsigned color = 0x000000);
};

