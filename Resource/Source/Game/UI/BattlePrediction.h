#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include "Status.h"
#include "MapChip.h"
#include "Dir.h"

class Charactor;

/// <summary>
/// �퓬�\��
/// </summary>
class BattlePrediction :
	public UI
{
private:
	// �U�����L�����N�^�[
	const Charactor& _selfCharactor;
	// ������L�����N�^�[
	const Charactor& _targetCharactor;
	// �U������
	unsigned int _distance;
	// HP�A�j���[�V�����g���b�N
	std::unique_ptr<Track<float>> _hpAnimTrack; 

	/// <summary>
	/// 
	/// </summary>
	/// <param name="dir"></param>
	/// <param name="rightAttack"></param>
	/// <param name="selfStatus"></param>
	/// <param name="targetStatus"></param>
	/// <returns></returns>
	int GetChengePoint(const Dir& dir, bool rightAttack, BattleStatus& selfStatus, BattleStatus& targetStatus);

public:
	/// <param name="self">�U�����L�����N�^�[</param>
	/// <param name="target">������L�����N�^�[</param>
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	/// <param name="attackStartPos">�U���J�n�ʒu</param>
	BattlePrediction(const Charactor& self, const Charactor& target, std::deque<std::shared_ptr<UI>>* uiDeque, const Vector2Int& attackStartPos);
	~BattlePrediction();

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
	/// 
	/// </summary>
	/// <param name="drawY"></param>
	/// <param name="windowRect"></param>
	/// <param name="rightAttack"></param>
	/// <param name="fontH"></param>
	void DrawHPBer(int& drawY, const Rect& windowRect, bool rightAttack, int fontH);
};

