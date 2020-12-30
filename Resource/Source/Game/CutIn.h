#pragma once
#include <memory>
#include <functional>
#include "CharactorType.h"
#include "Team.h"
#include "TimeLine.h"
#include "Dir.h"

/// <summary>
/// �J�b�g�C���Đ��N���X
/// </summary>
class CutIn
{
private:
	// �L�����N�^�[��
	const CharactorType _charactorType;
	// �`�[��
	const Team _team;
	// �g�k�A�j���[�V�����g���b�N
	std::unique_ptr<Track_f> _animTrack;
	// �ړ��A�j���[�V�����g���b�N
	std::unique_ptr<Track_f> _moveTrack;
	// �w�i�摜�n���h��
	int _bgH;
	// �J�b�g�C���T�C�Y
	Size _backImageSize;
	// �摜�n���h��
	int _graphH;
	// ���� �� : �U�����d�|������  �E : �U�����d�|����ꂽ��
	Dir _dir;
	// �A�j���[�V�����J�E���g
	int _animCnt;
	// �I������
	bool _end;
	// �I�����ɌĂԊ֐�
	std::function<void()> _endFunc;

public:
	/// <param name="type">�L�����N�^�[��</param>
	/// <param name="team">�`�[��</param>
	/// <param name="dir">���� �� : �U�����d�|������  �E : �U�����d�|����ꂽ��</param>
	/// <param name="endFunc">�I�����ɌĂԊ֐�</param>
	CutIn(const CharactorType type, const Team team, const Dir dir, std::function<void()> endFunc);
	~CutIn();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	bool GetEnd()const;
};

