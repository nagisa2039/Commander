#include "SlashingEffect.h"
#include "Animator.h"
#include "Charactor.h"
#include "BattleCharactor.h"

SlashingEffect::SlashingEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	:BattleEffect(self, target, effects, camera)
{
	_pos = _target.GetCenterPos().ToVector2();
	_size = Size(120, 120);
	_animator->SetImage("Resource/Image/Effect/slashingEffect.png");
	_animator->AddAnimDiv("normal", Rect(_size.ToVector2Int() * 0.5, _size), 9, 3, false, false);
	_animator->ChangeAnim("normal");

	AddDamage();
}

SlashingEffect::~SlashingEffect()
{
}
