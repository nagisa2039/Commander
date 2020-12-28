#pragma once
#include "Actor.h"
#include "Team.h"
#include <array>
#include <vector>
#include "TimeLine.h"

/// <summary>
/// �^�[���؂�ւ����̃A�j���[�V�����Đ��N���X
/// </summary>
class TurnChangeAnim
{
private:
	// ���O
	const char* name;
	// �`�[��
	Team _currentTeam;
	// �`�[�����Ƃ̏��Ǘ��z��
	std::array<const char*, static_cast<size_t>(Team::max)> teamInfs;
	// �A�j���[�V�����I������
	bool _animEnd;
	// SE�n���h��
	int _seH;

	// �g�嗦�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _exrateTL;
	// �s�����x�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _alphaTL;
	// ��]�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _angleTL;

public:
	TurnChangeAnim();
	~TurnChangeAnim();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �^�[���J�n���ɃA�j���[�V�������n�߂�
	/// </summary>
	/// <param name="team"></param>
	void TurnStart(const Team team);

	/// <summary>
	/// �I������擾
	/// </summary>
	bool GetAnimEnd()const;

	/// <summary>
	/// ���݂̃`�[���擾
	/// </summary>
	Team GetCurrentTeam()const;
};