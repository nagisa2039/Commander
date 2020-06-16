#include "BattleEffect.h"
#include "Charactor.h"
#include "BattleCharactor.h"
#include "Application.h"
#include "DataBase.h"
#include "../FlyText.h"

unsigned int  BattleEffect::AddDamage()
{
	auto selfBattleStatus = _self.GetCharacotr().GetBattleStatus();
	auto targetBattleStatus = _target.GetCharacotr().GetBattleStatus();
	bool critical = selfBattleStatus.GetCritical(targetBattleStatus) > rand() % 100;
	int damage = 0;
	if (selfBattleStatus.CheckHeal())
	{
		damage = -selfBattleStatus.GetRecover();
	}
	else
	{
		damage = selfBattleStatus.GetDamage(targetBattleStatus) * (critical ? 3 : 1);
	}

	char damageText[10];
	sprintf_s(damageText, 10, "%d", abs(damage));
	auto flyText = std::make_shared<FlyText>(damageText, _target.GetCenterPos(), 120, _camera, false, critical);
	_effects.emplace_back(flyText);
	_target.GetCharacotr().AddDamage(damage);
	if (selfBattleStatus.CheckHeal())
	{
		flyText->SetColor(0x80ff00);
	}
	return selfBattleStatus.CheckHeal() ? 0 : damage;
}

BattleEffect::BattleEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera, bool cameraActive)
	: Effect(self.GetCenterPos(), camera, cameraActive), _effects(effects), _self(self), _target(target)
{
}

BattleEffect::~BattleEffect()
{
}

void BattleEffect::Update(const Input& input)
{
	Effect::Update(input);
}

void BattleEffect::Draw()
{
	Effect::Draw();
}
