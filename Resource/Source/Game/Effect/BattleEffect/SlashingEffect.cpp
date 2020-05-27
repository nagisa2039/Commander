#include "SlashingEffect.h"
#include "Animator.h"

SlashingEffect::SlashingEffect(const Vector2Int& pos, const Vector2Int& targetPos, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	:BattleEffect(targetPos, targetPos, effects, camera)
{
	_size = Size(120, 120);
	_animator->SetImage("Resource/Image/Effect/slashingEffect.png");
	_animator->AddAnimDiv("normal", Rect(_size.ToVector2Int() * 0.5, _size), 9, 3, false, false);
	_animator->ChangeAnim("normal");
}

SlashingEffect::~SlashingEffect()
{
}