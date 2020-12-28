#pragma once
#include "MapCursor.h"
#include <memory>
#include <deque>
#include "TimeLine.h"

class Charactor;
class TerrainInf;
class StatusInf;
class StatusWindow;
class UI;

/// <summary>
/// �퓬������ʂ̃J�[�\��
/// </summary>
class BattlePreparationCursor :
	public MapCursor
{
private:
	// �I�𒆂̃}�b�v���W  x�����̒l�̎��͖��I��
	Vector2Int _selectMapPos;
	// �g�k�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _exRateTrack;
	// �I������
	bool _end;
	// ���莞SE
	int _okH;
	// �L�����Z����SE
	int _canselH;
	// �n�`���\���N���X
	std::shared_ptr<TerrainInf> _terrainInf;
	// �X�e�[�^�X���\���N���X
	std::shared_ptr<StatusInf> _statusInf;
	// �X�e�[�^�X�E�B���h�E�\���N���X
	std::shared_ptr<StatusWindow> _statusWindow;

	/// <summary>
	/// �L�����N�^�[�I��
	/// </summary>
	void Select();

public:
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="camera">�J����</param>
	BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera);
	~BattlePreparationCursor();

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
	/// �o���\�}�X�̕\��
	/// </summary>
	void DrawSortieMass();

	/// <summary>
	/// �ړ��\�}�X�̕\��
	/// </summary>
	void DrawMovableMass();

	/// <summary>
	/// �I������擾
	/// </summary>
	bool GetEnd()const;

	/// <summary>
	/// �J�n���̏�����
	/// </summary>
	void Start();

	/// <summary>
	/// �ŏ��̃J�[�\�����W�ݒ�
	/// </summary>
	void SetCursorStartPosition();
};

