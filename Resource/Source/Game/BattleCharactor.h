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
	std::shared_ptr<Effect> _attackEffect;

	uint8_t _animHealth;
	int _animHealthCnt;

	unsigned int _givenDamage;	// �^�����_���[�W

	static int _hpDotMaskH;

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
	Vector2Int GetPos()const;	// �����̍��W
	Vector2Int GetStartPos()const;	// �J�n�ʒu�̑������W
	Charactor& GetCharacotr();
	const unsigned int& GetGivenDamage()const;
	BattleCharactor* GetTargetBattleCharactor();

	void SetStartPos(const Vector2& startPos);	// �`�悷����W(������)
	void SetDir(const Dir dir);	//  �U��������̐ݒ�
	void SetTargetCharactor(BattleCharactor* target);	// �키����̃|�C���^�[
	void SetGivenDamage(const unsigned int value);
	void AddGivenDamage(const unsigned int value);

	// �U���G�t�F�N�g�̍쐬
	virtual std::shared_ptr<Effect> CreateAttackEffect(std::vector<std::shared_ptr<Effect>>& effects) = 0;
	// �U���O��G�t�F�N�g�̍쐬
	std::shared_ptr<Effect> CreateMissEffect(const Vector2Int& effectPos);
};

