#include "BattleEffect.h"
#include "Charactor.h"
#include "BattleCharactor.h"
#include "Application.h"
#include "DataBase.h"
#include "../FlyText.h"
#include "FileSystem.h"
#include "SoundLoader.h"

unsigned int  BattleEffect::AddDamage()
{
	auto selfBattleStatus = _self.GetCharacotr().GetBattleStatus();
	auto targetBattleStatus = _target.GetCharacotr().GetBattleStatus();
	int damage = 0;
	if (selfBattleStatus.CheckHeal())
	{
		damage = -selfBattleStatus.GetRecover();
		_target.SetReceiveDamageType(BattleCharactor::damageType::none);
	}
	else
	{
		damage = selfBattleStatus.GetDamage(targetBattleStatus) * (_critical ? 3 : 1);
		_target.SetReceiveDamageType(_critical ? BattleCharactor::damageType::critical : BattleCharactor::damageType::damage);
	}

	char damageText[10];
	sprintf_s(damageText, 10, "%d", abs(damage));
	auto flyText = std::make_shared<PopupText>(damageText, _target.GetCenterPos(), _camera, false, _critical);
	_effects.emplace_back(flyText);
	_target.GetCharacotr().AddDamage(damage);
	if (selfBattleStatus.CheckHeal())
	{
		flyText->SetColor(0x80ff00);
	}
	return selfBattleStatus.CheckHeal() ? 0 : damage;
}

BattleEffect::BattleEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
	bool critical, Camera& camera, bool cameraActive)
	: Effect(self.GetCenterPos(), camera, cameraActive), _effects(effects), _self(self), _target(target), _critical(critical)
{
	auto& dataBase = DataBase::Instance();
	auto& weaponData = dataBase.GetWeaponData(self.GetCharacotr().GetStatus().weaponId);
	auto efkPath = dataBase.GetBattleEffectData(weaponData.effectType).seName;
	SoundL.PlaySE(efkPath.c_str());
}

BattleEffect::~BattleEffect()
{
}