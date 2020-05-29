#include "RecoverEffect.h"
#include "Animator.h"
#include "Charactor.h"
#include "BattleCharactor.h"

RecoverEffect::RecoverEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	:BattleEffect(self, target, effects, camera)
{
	_pos = _target.GetCenterPos().ToVector2();
	
	_animator->SetImage("Resource/Image/Effect/recoverEffect.png");
	auto graphSize = _animator->GetImageSize();
	_size = Size(graphSize.h, graphSize.h);
	_animator->AddAnimDiv("normal", Rect(_size.ToVector2Int() * 0.5, _size), 9, 4, false, false);
	_animator->ChangeAnim("normal");

	_self.AddGivenDamage(AddDamage());
}

RecoverEffect::~RecoverEffect()
{
}
