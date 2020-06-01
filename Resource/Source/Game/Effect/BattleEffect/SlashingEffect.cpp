#include "SlashingEffect.h"
#include "Animator.h"
#include "Charactor.h"
#include "BattleCharactor.h"

SlashingEffect::SlashingEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	:BattleEffect(self, target, effects, camera)
{
	_pos = _target.GetCenterPos().ToVector2();
	_animator->SetImage("Resource/Image/Effect/slashingEffect2.png");
	_animator->AddAnimDiv("normal", Vector2Int(5,3), 2, false);
	_animator->ChangeAnim("normal");

	_self.AddGivenDamage(AddDamage());
}

SlashingEffect::~SlashingEffect()
{
}
