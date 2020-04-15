#pragma once
#include "Geometry.h"
#include "Dir.h"
#include <memory>
#include <string>

class Animator;
class Effect;
class Charactor;

class BattleScene;

class BattleCharactor
{
protected:
	Charactor& _selfChar;
	BattleCharactor* _targetChar;

	Dir _dir;
	Vector2 _startPos;
	Vector2 _pos;
	Size _size;
	std::shared_ptr<Animator> _animator;
	bool _createEffect;

	Size _uiSize;

	int _attackAnimCnt;
	int _attackAnimCntMax;

	uint8_t _animHealth;
	int _animHealthCnt;

	std::string _name;

public:
	BattleCharactor(Charactor& charactor, const int imageHandle);
	~BattleCharactor();

	virtual void Init(const Vector2& startPos, const Dir dir, BattleCharactor* target);

	virtual void AnimUpdate();
	virtual void AttackUpdate(BattleScene& battleScene);
	virtual void Draw();

	virtual void UIAnimUpdate();
	virtual void UIDraw();

	void StartAttackAnim();
	bool GetAttackAnimEnd();

	void StartHPAnim();
	bool GetHPAnimEnd();


	Size GetSize()const;
	Vector2Int GetCenterPos()const;
	Charactor& GetSelfCharacotr();

	void SetStartPos(const Vector2& startPos);	// �`�悷����W(������)
	void SetDir(const Dir dir);	//  �U��������̐ݒ�
	void SetTargetCharactor(BattleCharactor* target);	// �키����̃|�C���^�[

	virtual std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos) = 0;
};

