#pragma once
#include <vector>
#include <memory>
#include <deque>
#include <functional>
#include "Scene.h"
#include "Geometry.h"
#include "TimeLine.h"

class Camera;
class MapSelectCharactor;
class UI;

/// <summary>
/// �}�b�v�I���V�[��
/// </summary>
class MapSelectScene :
    public Scene
{
private:
	// ���ڃT�C�Y
	const Size _contentSize;
	// ���ڍ��W�z��
	std::vector<Vector2Int> _contentPosVec;
	// �J����
	std::unique_ptr<Camera> _camera;
	// �L�����N�^�[�Ǘ��z��
	std::vector<std::unique_ptr<MapSelectCharactor>> _mapSelectCharactors;
	// �t�F�[�h�I�����ɌĂ΂��֐�
	std::function<void()> _fadeEndFunc;
	// UI�e�q�֌W�Ǘ�Deque
	std::deque<std::shared_ptr<UI>> _uiDeque;

	// �I�𒆂̃C���f�b�N�X
	unsigned int _selectIdx;

	// �L�����N�^�[�ړ��J�n�܂ł̃C���^�[�o���g���b�N
	std::unique_ptr<Track<int>> _moveStartTrack;
	// �I�𒆍��ڃA�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _selectAnimTrack;
	// �ړ�������L�����N�^�[�C���f�b�N�X
	unsigned int _charactorIdx;

	// �L�����N�^�[�ړ����� -1 or +1
	int _dir;
	// BGM�n���h��
	int _bgmH;

	// �}�b�v�^�C�g�����n�n���h��
	int _uiH;

	// �`���X�N���[��
	int _screenH;

	// �X�V�֐��|�C���^
	void(MapSelectScene::* _updater)(const Input&);

	/// <summary>
	/// �ʏ�X�V
	/// </summary>
	/// <param name="input">�X�V</param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// �t�F�[�h���X�V
	/// </summary>
	/// <param name="input">�X�V</param>
	void FadeUpdate(const Input& input);

	/// <summary>
	/// �ړ��X�V
	/// </summary>
	/// <param name="input">�X�V</param>
	void MoveUpdate(const Input& input);

	/// <summary>
	/// �J�[�\���ړ��X�V
	/// </summary>
	/// <param name="input">�X�V</param>
	void CursorMove(const Input& input);

	/// <summary>
	/// �L�����N�^�[�ړ��X�V
	/// </summary>
	void MapSelectCharactorUpdate();

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="debug">�f�o�b�O��</param>
	void Decide(const bool debug = false);

	/// <summary>
	/// �L�����N�^�[�I���C���f�b�N�X�̐ݒ�
	/// </summary>
	/// <param name="value">�L�����N�^�[�I���C���f�b�N�X</param>
	void SetSelectIdx(const unsigned int value);

public:
	/// <param name="controller">�V�[���Ǘ��N���X</param>
	MapSelectScene(SceneController& controller);
	~MapSelectScene();

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