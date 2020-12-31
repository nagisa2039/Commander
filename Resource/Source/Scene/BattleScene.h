#pragma once
#include "Scene.h"
#include "../Utility/Dir.h"
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>
#include <deque>
#include "TimeLine.h"

class UI;
class Camera;
class Effect;
class Charactor;
class BattleCharactor;
class CutIn;
class PlayScene;

/// <summary>
/// �퓬�V�[��
/// </summary>
class BattleScene :
	public Scene
{
private:
	//�U�����d�|�������̃L�����N�^�[
	BattleCharactor& _leftBC;
	//�U�����d�|����ꂽ���̃L�����N�^�[
	BattleCharactor& _rightBC;

	// �`���X�N���[���T�C�Y
	const Size _screenSize;
	// �`���X�N���[���n���h��
	int _screenH;
	// �J����
	std::shared_ptr<Camera> _camera;

	// �G�t�F�N�g�Ǘ��z��
	std::vector<std::shared_ptr<Effect>> _effects;
	// �v���C�V�[��
	PlayScene* _playScene;

	// PlayScene�Ő퓬���N�������X�N���[�����W
	Vector2Int _startPos;
	// �n�ʂ̍���
	float _groundY;
	// �ǌ�������s������
	bool _pursuit;

	// BGM�n���h��
	int _bgmH;

	// �J�b�g�C���Đ��N���X
	std::shared_ptr<CutIn> _cutIn;

	// �g�k�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _exRateTL;
	// ��ʍ��x�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _brightTL;

	// �X�V�֐��|�C���^
	void(BattleScene::* _updater)(const Input& input);

	/// <summary>
	/// �퓬�J�n���̃A�j���[�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void SceneStartAnim(const Input& input);

	/// <summary>
	/// �I�����A�j���[�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void SceneEndAnim(const Input& input);

	/// <summary>
	/// �����^�[��
	/// </summary>
	/// <param name="input">���͏��</param>
	void LeftTurn(const Input& input);

	/// <summary>
	/// ����HP�A�j���[�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void LeftHPAnim(const Input& input);

	/// <summary>
	/// �E���^�[��
	/// </summary>
	/// <param name="input">���͏��</param>
	void RightTurn(const Input& input);

	/// <summary>
	/// �E��HP�A�j���[�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void RightHPAnim(const Input& input);

	/// <summary>
	/// �ǌ��ł���L����������Βǌ�����
	/// </summary>
	/// <param name="rightAttack">�E���̍U����������͈͂�</param>
	/// <returns>�ǌ��\</returns>
	bool PursuitAttack(const bool rightAttack = true);

	/// <summary>
	/// �V�[���I��
	/// PlayScene�ɖ߂�
	/// </summary>
	void End();

	/// <summary>
	/// ���̕`��
	/// </summary>
	/// <param name="screenCenter">�`��X�N���[�����S���W</param>
	void DrawFloor(Vector2Int& screenCenter);

public:
	/// <param name="leftBC">�U�����d�|�������̃L�����N�^�[</param>
	/// <param name="rightBC">�U�����d�|����ꂽ���̃L�����N�^�[</param>
	/// <param name="ctrl">�V�[���Ǘ��N���X</param>
	/// <param name="cameraOffset">�`��I�t�Z�b�g</param>
	BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl, const Vector2Int& cameraOffset);
	~BattleScene();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input) override final;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(void) override final;

	/// <summary>
	/// �G�t�F�N�g�Ǘ��z��̎擾
	/// </summary>
	std::vector<std::shared_ptr<Effect>>& GetEffectVec();

	/// <summary>
	/// �N���e�B�J���J�b�g�C�����Đ�����
	/// </summary>
	/// <param name="c">�N���e�B�J���J�b�g�C��</param>
	void SetCutIn(std::shared_ptr<CutIn> c);
};