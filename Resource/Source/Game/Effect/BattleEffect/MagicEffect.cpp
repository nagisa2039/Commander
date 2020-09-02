#include "MagicEffect.h"
#include "Animator.h"
#include "Charactor.h"
#include "BattleCharactor.h"

MagicEffect::MagicEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
	bool critical, Camera& camera)
	:BattleEffect(self, target, effects, critical, camera)
{
	_pos = _target.GetCenterPos().ToVector2();
	_animator->SetImage("Resource/Image/Effect/magicEffect.png");
	auto graphSize = _animator->GetImageSize();
	_size = Size(graphSize.h, graphSize.h);
	_animator->AddAnimDiv("normal", Rect(_size.ToVector2Int() * 0.5, _size), graphSize.w / graphSize.h, 4, false, false);
	_animator->ChangeAnim("normal");

	_self.AddGivenDamage(AddDamage());
}

MagicEffect::~MagicEffect()
{
}

