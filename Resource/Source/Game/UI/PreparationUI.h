#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include <array>
#include <functional>

class BattlePreparationCursor;
class MapCtrl;
class Camera;
class PlayScene;

/// <summary>
/// �퓬�������
/// </summary>
class PreparationUI :
	public UI
{
private:
	/// <summary>
	/// ���ڎ��
	/// </summary>
	enum class Content
	{
		// �퓬�J�n
		start,
		// �z�u����
		placement,	
		// �틵�m�F
		warsituation,
		// �߂�
		back,
		max
	};

	/// <summary>
	/// ���ڏ��
	/// </summary>
	struct ContentInf
	{  
		// ���O
		std::string name;
		// �\�����W
		Vector2Int pos;
		// ������s�֐�
		std::function<void(void)> func;
		// �`���摜�n���h��
		int graphH = -1;
	};

	// ���ڃT�C�Y
	Size _contentSize;

	// �I�𒆂̍���
	Content _selectContent;

	// PlayScene�̎Q��
	PlayScene& _playScene;
	// �J����
	Camera& _camera;
	// �}�b�v�Ǘ��N���X
	MapCtrl& _mapCtrl;

	// �J��SE�n���h��
	int _openH;
	// ����SE�n���h��
	int _closeH;
	// �I���ړ���SE�n���h��
	int _moveH;
	// ���莞SE�n���h��
	int _okH;
	// �L�����Z����SE�n���h��
	int _canselH;
	// BGM�n���h��
	int _bgmH;

	// ��������̌�����s���Ă��邩
	bool _execution;
	// �}�b�v�I���ɖ߂��Ă悢���̃t���O
	bool _backMapSelect;

	// �X�V�֐��|�C���^
	void (PreparationUI::* _updater)(const Input& input);

	// ���ڏ��Ǘ��z��
	std::array<ContentInf, static_cast<size_t>(Content::max)> _contentInfTable;

	// �J�A�j���[�V�����p�g���b�N
	std::unique_ptr<Track<float>> _openAnimTrack;
	// ���ڑI�����g�k�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _selectExRateTrack;
	// �o���}�X�I�����̃J�[�\��
	std::unique_ptr<BattlePreparationCursor> _battlePreparationCursor;

	/// <summary>
	/// ���؂����̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void CloseUpdate(const Input& input);

	/// <summary>
	/// �J���؂������̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void OpenUpdate(const Input& input);

	/// <summary>
	/// ����A�j���[�V�����X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void CloseAnimUpdate(const Input& input);

	/// <summary>
	/// �J���A�j���[�V�����X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void OpenAnimUpdate(const Input& input);

	/// <summary>
	/// ���؂����u�ԂɌĂ΂��
	/// </summary>
	void CloseEnd();

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="camera"></param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="playScene"></param>
	PreparationUI(std::deque<std::shared_ptr<UI>>* uiDeque, Camera& camera, MapCtrl& mapCtrl, PlayScene& playScene);
	~PreparationUI();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input)override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// �L�����N�^�[�`��̑O�ɍs��
	/// �ړ��\�}�X��o���\�}�X�̕`��
	/// </summary>
	void BeginDraw();

	/// <summary>
	/// �J��
	/// </summary>
	/// <param name="animation">�A�j���[�V�������s����</param>
	void Open(const bool animation);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="animation">�A�j���[�V�������s����</param>
	void Close(const bool animation);

	/// <summary>
	/// BGM�̊J�n
	/// </summary>
	void StartBGM();

	/// <summary>
	/// �}�b�v�I���ɖ߂邽�߂̃t���O�Z�b�g
	/// </summary>
	void BackMapSelect();

	/// <summary>
	/// �}�b�v�I���ɖ߂邩�̃t���O�擾
	/// </summary>
	/// <returns></returns>
	bool GetBackMapSelect();
};