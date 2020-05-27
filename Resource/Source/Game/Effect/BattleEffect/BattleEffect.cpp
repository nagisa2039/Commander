#include "BattleEffect.h"
#include "Charactor.h"
#include "BattleCharactor.h"
#include "Application.h"
#include "DataBase.h"
#include "../FlyText.h"

void BattleEffect::AddDamage()
{
	auto selfStatus = _self.GetCharacotr().GetStatus();
	auto targetStatus = _target.GetCharacotr().GetStatus();
	bool critical = selfStatus.GetCritical(targetStatus) > rand() % 100;
	int damage = 0;
	if (selfStatus.heal)
	{
		damage = -selfStatus.GetRecover();
	}
	else
	{
		damage = selfStatus.GetDamage(targetStatus) * (critical ? 3 : 1)
			* Application::Instance().GetDataBase().GetAttributeRate(selfStatus.attribute, targetStatus.attribute);
	}

	char damageText[10];
	sprintf_s(damageText, 10, "%d", abs(damage));
	auto flyText = std::make_shared<FlyText>(damageText, _target.GetCenterPos(), 120, _camera, false, critical);
	_effects.emplace_back(flyText);
	_target.GetCharacotr().AddDamage(damage);
	if (selfStatus.heal)
	{
		flyText->SetColor(0x80ff00);
	}
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
