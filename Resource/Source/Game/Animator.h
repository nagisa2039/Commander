#pragma once
#include <vector>
#include <map>
#include "../Utility/Geometry.h"
#include <string>

/// <summary>
/// �A�j���[�V�����Ǘ��N���X
/// </summary>
class Animator
{
public:
	/// <summary>
	/// �A�j���[�V�������
	/// </summary>
	struct AnimInfo
	{
		// �\����`�z��
		std::vector<Rect> rectVec;
		// �؂�ւ�����
		int itv;
		// ���[�v���邩
		bool isLoop;
	};

private:
	// �摜�̊i�[
	int _imageH;
	// �摜�T�C�Y
	Size _imageSize;
	// �A�j���[�V�����z��
	std::map<std::string, AnimInfo> _animInfoMap;

	// �A�j���[�V������
	std::string _animName;
	// ���݂̃t���[����
	int _animFrame;
	// �I������
	bool _animEnd;
	// Y�����]
	bool _reverseY;	

public:
	Animator();
	~Animator();

	/// <summary>
	/// �摜�n���h���̐ݒ�
	/// </summary>
	/// <param name="path">�p�X</param>
	void SetImage(const char* path);

	/// <summary>
	/// �摜�n���h���̐ݒ�
	/// </summary>
	/// <param name="handle">�n���h��</param>
	void SetImageHandle(const int handle);

	/// <summary>
	/// Y�����]�ݒ�
	/// </summary>
	/// <param name="value">Y�����]</param>
	void SetReverseY(const bool value);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="pos">�\�����W</param>
	void Draw(const Vector2Int& pos);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="pos">�\�����W</param>
	/// <param name="size">�\���T�C�Y</param>
	void Draw(const Vector2Int& pos, Size size);

	/// <summary>
	/// �A�j���[�V�����̒ǉ�
	/// </summary>
	/// <param name="animName">���O</param>
	/// <param name="animInfo">�A�j���[�V�������</param>
	void AddAnim(const std::string& animName, const AnimInfo& animInfo);

	/// <summary>
	/// �A�j���[�V�����̒ǉ�
	/// </summary>
	/// <param name="animName">���O</param>
	/// <param name="rectvec">��`�z��</param>
	/// <param name="animItv">�Ԋu</param>
	/// <param name="loop">���[�v</param>
	void AddAnim(const std::string& animName, const std::vector<Rect>& rectvec, const int animItv, const bool loop);

	/// <summary>
	/// �A�j���[�V�����̒ǉ�
	/// </summary>
	/// <param name="animName">���O</param>
	/// <param name="startRect">��`�J�n�ʒu</param>
	/// <param name="animCnt">�R�}��</param>
	/// <param name="animItv">�Ԋu</param>
	/// <param name="loop">���[�v</param>
	/// <param name="vertical">�c���т�</param>
	void AddAnimDiv(const std::string& animName, const Rect& startRect, const int animCnt, const int animItv, const bool loop, const bool vertical = true);

	/// <summary>
	/// �A�j���[�V�����̒ǉ�
	/// </summary>
	/// <param name="animName">���O</param>
	/// <param name="animCnt">�R�}��</param>
	/// <param name="animItv">�Ԋu</param>
	/// <param name="loop">���[�v</param>
	void AddAnimDiv(const std::string& animName, const Vector2Int& animCnt, const int animItv, const bool loop);

	/// <summary>
	/// �A�j���[�V�����̕ύX
	/// </summary>
	/// <param name="animName">�A�j���[�V������</param>
	void ChangeAnim(std::string animName);

	/// <summary>
	/// �A�j���[�V�������͂��߂����蒼��
	/// </summary>
	void AnimRestart();

	/// <summary>
	/// �A�j���[�V�������擾
	/// </summary>
	const std::string& GetAnimName()const;

	/// <summary>
	/// �I������
	/// </summary>
	const bool GetAnimEnd()const;

	/// <summary>
	/// �摜�n���h���̎擾
	/// </summary>
	const int GetAnimHandle()const;

	/// <summary>
	/// �A�j���[�V������`�擾
	/// </summary>
	const Rect& GetAnimRect()const;

	/// <summary>
	/// �摜�n���h���̎擾
	/// </summary>
	const int GetImageH()const;

	/// <summary>
	/// �摜�T�C�Y�̎擾
	/// </summary>
	const Size& GetImageSize()const;
};

