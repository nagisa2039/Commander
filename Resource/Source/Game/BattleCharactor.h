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
class FileSystem;

class BattleScene;

class BattleCharactor
{
public:
	enum class damageType
	{
		none,		// 攻撃を受けていない
		damage,		// 攻撃を受けている
		critical,	// 会心攻撃を受けている
		max
	};

protected:
	Charactor& _selfChar;
	BattleCharactor* _targetChar;
	Camera& _camera;

	Dir _dir;
	Vector2 _startPos;
	Vector2 _pos;
	Size _size;
	std::shared_ptr<Animator> _animator;

	std::unique_ptr<Track<int>> _attackAnimX;
	std::shared_ptr<Effect> _attackEffect;

	uint8_t _animHealth;
	int _animHealthCnt;

	unsigned int _givenDamage;	// 与えたダメージ

	static int _hpDotMaskH;

	damageType _damageType;

	void DrawName(const char* teamString, Rect& nameWindowRect, FileSystem& fileSystem, int fontHandle);
	void DrawParameter(const char* teamString, Rect& windowRect, FileSystem& fileSystem, Rect& paramWindowRect);
	void DrawHP(Rect& windowRect, int fontHandle);
	void DrawWeaponName(FileSystem& fileSystem, Rect& weaponNameRect);

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
	const unsigned int& GetGivenDamage()const;
	BattleCharactor* GetTargetBattleCharactor();
	damageType GetDamageType()const;

	void SetStartPos(const Vector2& startPos);	// 描画する座標(中央下)
	void SetDir(const Dir dir);	//  攻撃か守備の設定
	void SetTargetCharactor(BattleCharactor* target);	// 戦う相手のポインター
	void SetDamageType(const damageType dt);
	void SetGivenDamage(const unsigned int value);
	void AddGivenDamage(const unsigned int value);

	// 攻撃エフェクトの作成
	virtual std::shared_ptr<Effect> CreateAttackEffect(std::vector<std::shared_ptr<Effect>>& effects) = 0;
	// 攻撃外れエフェクトの作成
	std::shared_ptr<Effect> CreateMissEffect(const Vector2Int& effectPos);
};

