#pragma once
#include <memory>
#include <list>
#include <vector>
#include <array>
#include "Scene.h"
#include "../Utility/Geometry.h"
#include "../Game/Team.h"
#include <list>
#include <string>
#include "TimeLine.h"
#include <functional>

class Charactor;
class MapCtrl;
class Camera;
class Commander;
class Effect;
class TurnChangeAnim;
class PreparationUI;
class UI;

/// <summary>
/// �v���C�V�[��
/// </summary>
class PlayScene :
	public Scene
{
public:
	/// <summary>
	/// �`�掞�ɂ�����t�B���^�[��
	/// </summary>
	enum class FilterType
	{
		// �����Ȃ�
		none,
		// �ڂ���
		gauss,
		max
	};
private:
	// AI���m�̐퓬
	bool _aiMode;
	// �`���X�N���[���n���h��
	int _gameH;
	
	// �}�b�v�Ǘ��N���X
	std::unique_ptr<MapCtrl> _mapCtrl;
	// �J����
	std::unique_ptr<Camera> _camera;

	// �^�[����
	unsigned char _turnCnt;
	// �����^�[����
	const unsigned char _limitTurnCnt;

	// �^�[���J�ڃA�j���[�V����
	std::unique_ptr<TurnChangeAnim> _turnChangeAnim;

	// �L�����N�^�[�Ǘ��z��
	std::vector<std::shared_ptr<Charactor>> _charactors;
	// ���S�L�����N�^�[�C�e���[�^�[
	std::vector<std::shared_ptr<Charactor>>::iterator _dyingCharItr;
	// �v���C���[�w����
	std::unique_ptr<Commander> _playerCommander;
	// �G�w����
	std::unique_ptr<Commander> _enemyCommander;
	// �G�t�F�N�g�Ǘ��z��
	std::vector<std::shared_ptr<Effect>> _effects;

	// �����I�ɏI������Ƃ���UI�p
	std::deque<std::shared_ptr<UI>> _endUIDeque;
	// �퓬����UI�e�q�֌W�Ǘ�Deque
	std::deque<std::shared_ptr<UI>> _preparationDeque;
	// �퓬�������
	std::shared_ptr<PreparationUI> _preparationUI;

	// �N���A�A�j���[�V�����p�g���b�N
	std::unique_ptr<Track_f> _clearAnimTrack;
	// �f���A�j���[�V�����p�g���b�N
	std::unique_ptr<Track_f> _demoAnimTrack;
	// �����V�[���؂�ւ��p�g���b�N
	std::unique_ptr<Track_i> _autoSceneChangeTrack;

	// �t�F�[�h�I�����ɌĂ΂��֐�
	std::function<void()>_fadeEndFunc;

	// �\�����t�B���^�[��
	FilterType _filterType;
	// �t�B���^�[���Ƃ̎��s�֐��e�[�u��
	std::array<std::function<void()>, static_cast<size_t>(FilterType::max)> _filterFuncs;

	// ��ʂ��Ƃ̍X�V���s���֐��|�C���^ playScene���p�����邩��Ԃ�
	void(PlayScene::*_updater)(const Input& input);
	// �ꎞ�I��Update�֐���ς���Ƃ��ɑOUpdate�֐����i�[����
	void(PlayScene::* _updaterOld)(const Input& input);

	// ��ʂ��Ƃ̍X�V���s���֐��|�C���^
	void(PlayScene::* _drawer)();
	// UI�`��֐��|�C���^
	void(PlayScene::* _UIDrawer)();

	/// <summary>
	/// �v���C���[�^�[�����J�n����
	/// </summary>
	void StartPlayerTurn();

	/// <summary>
	/// �G�^�[�����J�n����
	/// </summary>
	void StartEnemyTurn();

	/// <summary>
	/// �Q�[�����J�n����
	/// </summary>
	void GameStart();

	/// <summary>
	/// �������
	/// </summary>
	/// <param name="input">���͏��</param>
	void PreparationUpdate(const Input& input);

	/// <summary>
	/// �^�[���ړ����X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void TurnChengeUpdate(const Input& input);

	/// <summary>
	/// �v���C���[�^�[�����X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void PlayerTurnUpdate(const Input& input);

	/// <summary>
	/// �L�����N�^�[�X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void CharactorUpdate(const Input& input);

	/// <summary>
	/// �G�^�[���X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void EnemyTurnUpdate(const Input& input);

	/// <summary>
	/// PlayScene���甲����Ƃ��̓��͎�t
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <returns>������</returns>
	bool BackSceneWindow(const Input& input);

	/// <summary>
	/// �L�����N�^�[���S���X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void CharactorDyingUpdate(const Input& input);

	/// <summary>
	/// �Q�[���N���A���X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void GameClearUpdate(const Input& input);

	/// <summary>
	/// �Q�[���I�[�o�[���X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void GameOverUpdate(const Input& input);

	/// <summary>
	/// �Q�[���N���A�Ɉڍs
	/// </summary>
	void GameClear();

	/// <summary>
	/// �Q�[���I�[�o�[�Ɉڍs
	/// </summary>
	void GameOver();

	/// <summary>
	/// �t�F�[�h�C�����J�n����
	/// </summary>
	/// <param name="funcP">�t�F�[�h�I�����ɌĂԊ֐�</param>
	/// <param name="color">�t�F�[�h���̐F</param>
	/// <param name="nextDrawer">���̕`��֐�</param>
	void StartFadeIn(std::function<void()>funcP, const unsigned int color = 0x000000, void(PlayScene::* nextDrawer)() = &PlayScene::BaseDraw);

	/// <summary>
	/// �t�F�[�h�A�E�g���J�n����
	/// </summary>
	/// <param name="funcP">�t�F�[�h�I�����ɌĂԊ֐�</param>
	/// <param name="color">�t�F�[�h���̐F</param>
	/// <param name="nextDrawer">���̕`��֐�</param>
	void StartFadeOut(std::function<void()> funcP, const unsigned int color = 0x000000, void(PlayScene::* nextDrawer)() = &PlayScene::BaseDraw);

	/// <summary>
	/// �t�F�[�h�X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void FadeUpdate(const Input& input);

	/// <summary>
	/// �퓬�������̕`��
	/// </summary>
	void PreparationDraw();

	/// <summary>
	/// �^�[���ڍs�̃A�j���[�V�����`��
	/// </summary>
	void TurnChengeDraw();

	/// <summary>
	/// �v���C���[�^�[�����̕`��
	/// </summary>
	void PlayerTurnDraw();

	/// <summary>
	/// �G�^�[�����̕`��
	/// </summary>
	void EnemyTurnDraw();

	/// <summary>
	/// �Q�[���I�[�o�[���̕`��
	/// </summary>
	void GameOverDraw();

	/// <summary>
	/// �Q�[���N���A���̕`��
	/// </summary>
	void GameClearDraw();

	/// <summary>
	/// �ʏ�`��
	/// </summary>
	void BaseDraw();

	/// <summary>
	/// �퓬�����̕`��
	/// </summary>
	void PreparationUIDraw();

	/// <summary>
	/// �v���C���[UI�̕`��
	/// </summary>
	void PlayerUIDraw();

	/// <summary>
	/// �ʏ�UI�̕`��
	/// </summary>
	void NoneUIDraw();

	// Fade�I�����Ɏ��s����֐�-------------------------------------------------------
	/// <summary>
	/// �^�C�g���V�[���ւ̈ڍs
	/// </summary>
	void ChengeTitle();

	/// <summary>
	/// �}�b�v�I���V�[���ւ̈ڍs
	/// </summary>
	void ChengeMapSelect();

	/// <summary>
	/// �퓬������ʂւ̈ڍs
	/// </summary>
	void ChangePreparation();

	/// <summary>
	/// �Q�[���I�[�o�[�ւ̈ڍs
	/// </summary>
	void ChangeGameOver();

public:
	/// <param name="ctrl">�V�[���Ǘ��N���X</param>
	/// <param name="mapId">�}�b�vID</param>
	/// <param name="ai">AI���m�̑Ό���</param>
	PlayScene(SceneController & ctrl, const unsigned int mapId, const bool ai = false);
	~PlayScene();

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
	/// �V�[�����A�N�e�B�u�ɂȂ�����Ă΂��
	/// </summary>
	void On()override;

	/// <summary>
	/// �V�[������A�N�e�B�u�ɂȂ�����Ă΂��
	/// </summary>
	void Off()override;

	/// <summary>
	/// �`�掞�Ƀt�B���^�[��������
	/// </summary>
	/// <param name="type">�t�B���^�[��</param>
	void SetFilter(const FilterType type);
};