#pragma once
#include <memory>
#include "Effect.h"
#include "DxLibUtility.h"
#include "TimeLine.h"

/// <summary>
/// �e�L�X�g�G�t�F�N�g
/// </summary>
class TextEffect :
	public Effect
{
protected:
	// �`�悷�镶����
	std::string _str;
	// �t�H���g�n���h��
	int _fontHandle;
	// �F
	unsigned int _color;

public:
	/// <param name="str">�`�悷�镶����</param>
	/// <param name="pos">���W</param>
	/// <param name="camera">�J����</param>
	/// <param name="cameraActive">�J�������l�����ĕ`�悷�邩</param>
	TextEffect(const std::string& str, const Vector2Int& pos, 
		Camera& camera, const bool cameraActive);
	virtual ~TextEffect() = default;

	/// <summary>
	/// �t�H���g�n���h���̐ݒ�
	/// </summary>
	/// <param name="handle">�t�H���g�n���h��</param>
	void SetFontHandle(const int handle);

	/// <summary>
	/// �F�̐ݒ�
	/// </summary>
	/// <param name="color">�F</param>
	void SetColor(const unsigned int color);
};

/// <summary>
/// �㏸���Ă����e�L�X�g�G�t�F�N�g
/// </summary>
class FlyText :
	public TextEffect
{
private:
	// �`�悷��
	Anker _anker;
	// �ړ���
	Vector2 _move;
	// �N���e�B�J���t���O
	bool _critical;
	// �ړ��A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _moveTrack;
	// ���߃A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _alphaTrack;

public:
	/// <param name="str">�`�悷�镶����</param>
	/// <param name="pos">���W</param>
	/// <param name="lifeCnt">�����t���[����</param>
	/// <param name="camera">�J����</param>
	/// <param name="cameraActive">�J�������l�����ĕ`�悷�邩</param>
	/// <param name="critical">�N���e�B�J���t���O</param>
	FlyText(const std::string& str, const Vector2Int& pos, const int lifeCnt, 
		Camera& camera, const bool cameraActive, const bool critical = false);
	~FlyText();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(const Input& input) override;

	/// <summary>
	///�@�`��
	/// </summary>
	virtual void Draw() override;

	/// <summary>
	///  �`�悷����ݒ�
	/// </summary>
	/// <param name="anker">�</param>
	void SetAnker(const Anker anker);

	/// <summary>
	/// �ړ��͂�ݒ�
	/// </summary>
	/// <param name="move"></param>
	void SetMove(const Vector2& move);
};

/// <summary>
/// ���˂�e�L�X�g�G�t�F�N�g
/// </summary>
class PopupText :
	public TextEffect
{
private:
	// �������
	struct charInf
	{
		// ����
		char c;
		// �J�n���W
		Vector2Int startPos;
		// ���݂�Y���W
		float currentY;
		// ���x
		float velocityY;
		// �I������
		bool end;
	};

	// ������̏��Ǘ��z��
	std::vector<charInf> _charInfs;
	// �ړ��A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _moveTrack;
	// ���߃A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _alphaTrack;
	// ���˂����镶���ԍ�
	int _currentIdx;

	// �`�悷��
	Anker _anker;
	// �N���e�B�J���t���O
	bool _critical;

public:
	/// <param name="str">�`�悷�镶����</param>
	/// <param name="pos">���W</param>
	/// <param name="camera">�J����</param>
	/// <param name="cameraActive">�J�������l�����ĕ`�悷�邩</param>
	/// <param name="critical">�N���e�B�J���t���O</param>
	PopupText(const std::string& str, const Vector2Int& pos, 
		Camera& camera, bool cameraActive, bool critical = false);
	virtual ~PopupText() = default;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(const Input& input) override;

	/// <summary>
	///�@�`��
	/// </summary>
	virtual void Draw() override;
};