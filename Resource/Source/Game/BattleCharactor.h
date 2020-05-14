#pragma once
#include "Geometry.h"
#include "Dir.h"
#include <memory>
#include <string>
#include "TimeLine.h"

class Animator;
class Effect;
class Charactor;
class Camera;

class BattleScene;

class BattleCharactor
{
protected:
	Charactor& _selfChar;
	BattleCharactor* _targetChar;
	Camera& _camera;

	Dir _dir;
	Vector2 _startPos;
	Vector2 _pos;
	Size _size;
	std::shared_ptr<Animator> _animator;

	Size _uiSize;

	std::unique_ptr<Track<int>> _attackAnimX;

	uint8_t _animHealth;
	int _animHealthCnt;

public:
	BattleCharactor(Charactor& charactor, const int imageHandle, Camera& camera);
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
	Vector2Int GetPos()const;	// 足元の座標
	Vector2Int GetStartPos()const;	// 開始位置の足元座標
	Charactor& GetCharacotr();

	void SetStartPos(const Vector2& startPos);	// 描画する座標(中央下)
	void SetDir(const Dir dir);	//  攻撃か守備の設定
	void SetTargetCharactor(BattleCharactor* target);	// 戦う相手のポインター

	virtual std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos) = 0;
};

