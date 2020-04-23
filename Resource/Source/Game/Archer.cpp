#include "Archer.h"
#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"
#include "Animator.h"
#include "../Scene/SceneController.h"
#include "SlashingEffect.h"
#include "Application.h"
#include <DxLib.h>
#include "FileSystem.h"

using namespace std;

Archer::Archer(const uint8_t level, const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl,
	std::vector<std::shared_ptr<Effect>>& effects)
	:Charactor(level, mapPos, team, mapCtrl, ctrl, effects)
{
	const Size divSize = Size(32, 32);
	if (_team == Team::player)
	{
		_animator->SetImage("Resource/Image/Charactor/archer_player.png");
	}
	else
	{
		_animator->SetImage("Resource/Image/Charactor/archer_enemy.png");
	}
	_battleC = make_shared<ArcherBC>(*this, _animator->GetImageH());

	InitAnim();

	_status = Status(level, 20, 10, 5, 5, 5, 5, 5, Attribute::red);
	_status.health += level * 0.6f;
	_status.power += level * 0.5f;
	_status.defense += level * 0.6f;
	_status.speed += level * 0.4f;
	_status.skill += level * 0.5f;
	_status.luck += level * 0.6f;
	_startStatus = _status;

	_iconPath = "Resource/Image/Icon/archerIcon.png";
	_attackRange = Range(2, 2);
}

Archer::~Archer()
{
}

ArcherBC::ArcherBC(Charactor& charactor, const int imageHandle) : BattleCharactor(charactor, imageHandle)
{
	_name = "Archer";
	const Size divSize = Size(32, 32);
	_animator->SetImageHandle(imageHandle);

	int cnt = 0;
	auto nextRectCenterOffset = [&](std::vector<Rect>& animRectVec, int cnt)
	{
		for (auto& rect : animRectVec)
		{
			rect.center.y += divSize.h;
		}
	};

	std::vector<Rect> animRectVec;
	animRectVec.clear();

	animRectVec.emplace_back(Rect(Vector2Int(16, 16 + 32 * 1), divSize));
	animRectVec.emplace_back(Rect(Vector2Int(16 + divSize.w * 2, 16 + 32 * 1), divSize));
	_animator->AddAnim("LeftWalk", animRectVec, 30, true);
	animRectVec.clear();

	animRectVec.emplace_back(Rect(Vector2Int(16, 16 + 32 * 2), divSize));
	animRectVec.emplace_back(Rect(Vector2Int(16 + divSize.w * 2, 16 + 32 * 2), divSize));
	_animator->AddAnim("RightWalk", animRectVec, 30, true);

}

ArcherBC::~ArcherBC()
{
}

std::shared_ptr<Effect> ArcherBC::CreateAttackEffect(const Vector2Int& effectPos)
{
	return make_shared<SlashingEffect>(effectPos);
}
