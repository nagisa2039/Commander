#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "Scene.h"
#include "TimeLine.h"
#include "CharactorType.h"
#include "Team.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene :
    public Scene
{
private:
	// �w�i�摜
	int _bgH;
	// �J�n�A�j���[�V�����摜�p�n���h��
	int _start_bksH;
	// �^�C�g�����S�摜�n���h��
	int _titleH;
	// BGM
	int _bgmH;

	/// <summary>
	/// �w�i�ɍ~�炷�L�����N�^�[���
	/// </summary>
	struct CharInf
	{
		// �`�悷�邩
		bool active;
		// ���W
		Vector2 pos;
		// �`�[��
		Team team;
		// �L�����N�^�[��
		CharactorType type;
		// ��]
		float angle;
	};

	// �L�����N�^�[���Ǘ��z��
	std::vector<CharInf> _charInfVec;
	// �L�����N�^�[�����C���^�[�o���g���b�N
	std::unique_ptr<Track_f> _charCreateTrack;

	// �J�n�A�j���[�V�����p�g���b�N
	std::unique_ptr<Track_f> _animTrack;
	// �f���V�[���Z����܂ł̃J�E���g
	std::unique_ptr<Track_f> _demoSceneCnt;

	// �X�V�֐��|�C���^
	void(TitleScene::* _updater)(const Input&);

	// �t�F�[�g�I�����ɌĂ΂��֐�
	std::function<void()> _fadeEndFunc;

	/// <summary>
	/// �ʏ펞�̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// �t�F�[�g���̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void FadeUpdate(const Input& input);

	/// <summary>
	/// �L�����N�^�[�X�V
	/// </summary>
	void CharactorUpdate();

public:
	/// <param name="controller">�V�[���Ǘ��N���X</param>
	TitleScene(SceneController& controller);
	~TitleScene();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input) override final;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(void) override final;
};