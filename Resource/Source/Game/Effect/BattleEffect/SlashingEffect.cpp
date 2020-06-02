#include "SlashingEffect.h"
#include "Animator.h"
#include "Charactor.h"
#include "BattleCharactor.h"

SlashingEffect::SlashingEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	:BattleEffect(self, target, effects, camera)
{
	_pos = _target.GetCenterPos().ToVector2();
	Vector2Int animCnt(9, 1);
	_animator->SetImage("Resource/Image/Effect/slashingEffect.png");
	_animator->AddAnimDiv("normal", animCnt, 2, false);
	_animator->ChangeAnim("normal");
	_size = _animator->GetImageSize() / animCnt;

	_self.AddGivenDamage(AddDamage());
}

SlashingEffect::~SlashingEffect()
{
}
