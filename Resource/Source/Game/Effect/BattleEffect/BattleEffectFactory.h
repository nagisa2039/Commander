#pragma once
#include <memory>
#include <vector>
#include <array>
#include <functional>
#include "BattleEffectType.h"

class Effect;
class Camera;
class BattleCharactor;

class BattleEffectFactory
{
private:
	using CrateBattleEffectFunc_t = std::function<std::shared_ptr<Effect>(
		BattleCharactor&, BattleCharactor&, std::vector<std::shared_ptr<Effect>>&,bool, Camera&, bool)>;

	std::array<CrateBattleEffectFunc_t, static_cast<size_t>(BattleEffectType::max)> _createBattleEffectFuncs;

public:
	BattleEffectFactory();
	~BattleEffectFactory()=default;

	std::shared_ptr<Effect> CreateBattleEffect(const BattleEffectType type, BattleCharactor & self, BattleCharactor & target, std::vector<std::shared_ptr<Effect>> & effects,
		bool critical, Camera & camera, bool cameraActive = false)const;
};