#include "ArrowEffect.h"

ArrowEffect::ArrowEffect(const Vector2Int& pos, const Vector2Int& targetPos, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	: BattleEffect(pos, targetPos, effects, camera)
{
	_dir = (targetPos - pos).x > 0 ? Dir::left : Dir::right;
}

ArrowEffect::~ArrowEffect()
{
}

void ArrowEffect::Update(const Input& input)
{
	if (_delete)
	{
		return;
	}

	_delete = true;
}
