#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>

class PlayScene;
class MapCtrl;

/// <summary>
/// �틵�m�F���
/// </summary>
class WarSituation :
	public UI
{
private:
	// �}�b�v�Ǘ��N���X
	const MapCtrl& _mapCtrl;
	// �g�k�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _exRate;
	// �X�V�֐��|�C���^
	void(WarSituation::* _updater)(const Input& input);
	// �I������
	bool _end;
	// PlayScene
	PlayScene* _playScene;

	/// <summary>
	/// �g�k�X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void ScalingUpdate(const Input& input);

	/// <summary>
	/// �ʏ�X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void NormalUpdate(const Input& input);

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	WarSituation(std::deque<std::shared_ptr<UI>>* uiDeque, const MapCtrl& mapCtrl);
	~WarSituation();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(const Input& input)override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;
};