#pragma once
#include "Geometry.h"
#include "Dir.h"
#include <memory>
#include <string>
#include <array>
#include <functional>
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
		none,		// �������Ă��Ȃ�
		damage,		// ���Ă�
		critical,	// ��S�U���𓖂Ă�
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

	damageType _gaveDamageType;	// �U�������蔻��p
	using attackEffectFunc_t = std::function<void(BattleScene&, const Vector2Int&)>;
	std::array<attackEffectFunc_t, static_cast<size_t>(damageType::max)> _attackEffectFuncs;

	unsigned int _givenDamage;	// �^�����_���[�W

	static int _hpDotMaskH;

	damageType _receiveDamageType;

	void(BattleCharactor::* _updater)(BattleScene&);

	void NormalUpdate(BattleScene& battleScene);
	void CutInUpdate(BattleScene& battleScene);

	void DrawName(const char* teamString, Rect& nameWindowRect, FileSystem& fileSystem, int fontHandle);
	void DrawParameter(const char* teamString, Rect& windowRect, FileSystem& fileSystem, Rect& paramWindowRect);
	void DrawHP(Rect& windowRect, int fontHandle);
	void DrawWeaponName(FileSystem& fileSystem, Rect& weaponNameRect);

public:
	BattleCharactor(Charactor& charactor, const int imageHandle, Camera& camera);
	~BattleCharactor();

	void Init(const Vector2& startPos, const Dir dir, BattleCharactor* target);

	void AnimUpdate();
	void Update(BattleScene& battleScene);
	void Draw();

	void UIAnimUpdate();
	void UIDraw();

	void StartAttackAnim(BattleScene& battleScene);
	bool GetAttackAnimEnd();

	void StartHPAnim();
	bool GetHPAnimEnd();

	Size GetSize()const;
	Vector2Int GetCenterPos()const;
	Vector2Int GetPos()const;	// �����̍��W
	Vector2Int GetStartPos()const;	// �J�n�ʒu�̑������W
	Charactor& GetCharacotr();
	const unsigned int& GetGivenDamage()const;
	BattleCharactor* GetTargetBattleCharactor();
	damageType GetDamageType()const;

	void SetStartPos(const Vector2& startPos);	// �`�悷����W(������)
	void SetDir(const Dir dir);	//  �U��������̐ݒ�
	void SetTargetCharactor(BattleCharactor* target);	// �키����̃|�C���^�[
	void SetDamageType(const damageType dt);
	void SetGivenDamage(const unsigned int value);
	void AddGivenDamage(const unsigned int value);
};

