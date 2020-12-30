#include <cassert>
#include "BattleEffectFactory.h"
#include "Tool.h"
#include "ArrowEffect.h"
#include "SlashingEffect.h"
#include "MagicEffect.h"
#include "RecoverEffect.h"
#include "../MissEffect.h"

#include "BattleCharactor.h"
#include "Camera.h"

BattleEffectFactory::BattleEffectFactory()
{
	_createBattleEffectFuncs[Size_t(BattleEffectType::slash)] = [](BattleCharactor& self, BattleCharactor& target,
		std::vector<std::shared_ptr<Effect>>& effects, bool critical, Camera& camera, bool cameraActive)
	{ 
		std::shared_ptr<SlashingEffect> effect(new SlashingEffect(self, target, effects, critical, camera));
		return effect;
	};

	_createBattleEffectFuncs[Size_t(BattleEffectType::magic)] = [](BattleCharactor& self, BattleCharactor& target,
		std::vector<std::shared_ptr<Effect>>& effects, bool critical, Camera& camera, bool cameraActive)
	{
		return std::make_shared<MagicEffect>(self, target, effects, critical, camera); 
	};

	_createBattleEffectFuncs[Size_t(BattleEffectType::arrow)] = [](BattleCharactor& self, BattleCharactor& target,
		std::vector<std::shared_ptr<Effect>>& effects, bool critical, Camera& camera, bool cameraActive)
	{
		return std::make_shared<ArrowEffect>(self, target, effects, critical, camera); 
	};

	_createBattleEffectFuncs[Size_t(BattleEffectType::recover)] = [](BattleCharactor& self, BattleCharactor& target,
		std::vector<std::shared_ptr<Effect>>& effects, bool critical, Camera& camera, bool cameraActive)
	{
		return std::make_shared<RecoverEffect>(self, target, effects, critical, camera); 
	};

	_createBattleEffectFuncs[Size_t(BattleEffectType::miss)] = [](BattleCharactor& self, BattleCharactor& target,
		std::vector<std::shared_ptr<Effect>>& effects, bool critical, Camera& camera, bool cameraActive)
	{
		return std::make_shared<PopupMissEffect>(target.GetCenterPos(), camera, false);
	};
}

std::shared_ptr<Effect> BattleEffectFactory::CreateBattleEffect(const BattleEffectType type, BattleCharactor& self, BattleCharactor& target,
	std::vector<std::shared_ptr<Effect>>& effects, bool critical, Camera& camera, bool cameraActive)const
{
	assert(Size_t(type) < _createBattleEffectFuncs.size());
	return _createBattleEffectFuncs[Size_t(type)](self, target, effects, critical, camera, cameraActive);
}
