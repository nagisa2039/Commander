#pragma once
#include "Actor.h"
#include <string>

class MapCtrl;

/// <summary>
/// �}�b�v�J�[�\�����N���X
/// </summary>
class MapCursor :
	public Actor
{
private:
	/// <summary>
	/// �L�[�{�[�h�ł̐���
	/// </summary>
	/// <param name="input">���͏��</param>
	void KeybordMove(const Input& input);

	/// <summary>
	/// �}�E�X�ł̐���
	/// </summary>
	/// <param name="input">���͏��</param>
	void MouseMove(const Input& input);

	// �ړ��֐��p�֐��|�C���^
	void(MapCursor::* _cursorMover)(const Input&);

protected:
	// �}�b�v���W
	Vector2Int _mapPos;
	// �}�b�v�Ǘ��N���X
	MapCtrl& _mapCtrl;

	// ���͎�t�܂ł̃t���[����
	int _moveItv;
	// ���͎�t�܂ł̃t���[�����̍ő�l
	int _moveItvCurrentMax;
	// �ړ�����SE�n���h��
	int _moveSEH;
	// �ŏ��̓��͎�t�܂ł̃t���[�����̍ő�l
	const int _moveItvMax;

	// ������͎�t�܂ł̃t���[����
	int _putItv;
	// ������͎�t�܂ł̃t���[���ő吔
	int _putItvCurrentMax;
	// �ŏ��̌�����͎�t�܂ł̃t���[���ő吔
	const int _putItvMax;

	/// <summary>
	/// �ړ�
	/// </summary>
	/// <param name="input">���͏��</param>
	void CursorMove(const Input& input);

	/// <summary>
	/// �L�[�{�[�h���͂ɐ؂�ւ�
	/// </summary>
	void ChangeKeybordInput();

	/// <summary>
	/// �}�E�X���͂ɐ؂�ւ�
	/// </summary>
	void ChangeMouseInput();

	/// <summary>
	/// ������͂��󂯕t�����邩���m�F
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <param name="key">�m�F����{�^����</param>
	bool PutCheck(const Input& input, const std::string& key);

	/// <summary>
	/// �J�[�\�����ړ��u�Ԃ̃R�[���o�b�N
	/// </summary>
	virtual void CursorMoveMoment();

public:
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="camera">�J����</param>
	MapCursor(MapCtrl& mapCtrl, Camera& camera);
	virtual ~MapCursor();

	/// <summary>
	/// �}�b�v���W
	/// </summary>
	const Vector2Int& GetMapPos()const;

	/// <summary>
	/// �}�b�v���W�̐ݒ�
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	void SetMapPos(const Vector2Int& mapPos);

	/// <summary>
	/// ���S���W�̎擾
	/// </summary>
	Vector2 GetCenterPos()const override;
};