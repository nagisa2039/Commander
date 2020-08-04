#include "MapSelectCharactor.h"
#include "Camera.h"
#include "Animator.h"
#include "Team.h"
#include "DataBase.h"
#include "Application.h"
#include "Input.h"

using namespace std;

namespace
{
	constexpr unsigned int FRONT_CHANGE_ITV = 10;
}

MapSelectCharactor::MapSelectCharactor(Camera& camera, const CharactorType& charactorType): Actor(camera)
{
	_animator = make_unique<Animator>();
	const Size divSize = Size(32, 32);
	_animator->SetImageHandle(Application::Instance().GetDataBase().GetCharactorImageHandle(charactorType, Team::player));

	auto nextRectCenterOffset = [&](std::vector<Rect>& animRectVec)
	{
		for (auto& rect : animRectVec)
		{
			rect.center.y += divSize.h;
		}
	};
	std::vector<Rect> animRectVec;
	animRectVec.emplace_back(Rect(Vector2Int(16, 16), divSize));
	animRectVec.emplace_back(Rect(Vector2Int(16 + divSize.w * 2, 16), divSize));

	nextRectCenterOffset(animRectVec);
	_animator->AddAnim("LeftWalk", animRectVec, 30, true);
	nextRectCenterOffset(animRectVec);
	_animator->AddAnim("RightWalk", animRectVec, 30, true);
	nextRectCenterOffset(animRectVec);
	_animator->AddAnim("UpWalk", animRectVec, 30, true);
	animRectVec.clear();
	animRectVec.emplace_back(Rect(Vector2Int(static_cast<int>(divSize.w * 1.5f), static_cast<int>(divSize.h * 0.5f)), divSize));
	_animator->AddAnim("DownWalk", animRectVec, 30, false);

	_animator->ChangeAnim("DownWalk");

	_speed = 10;
	_isMove = false;
}

MapSelectCharactor::~MapSelectCharactor()
{
}

void MapSelectCharactor::Update(const Input& input)
{
	if (!_isMove)
	{
		const char* downWalk = "DownWalk";
		if (_animator->GetAnimName() != downWalk && --_fontChangeCnt <= 0)
		{
			_animator->ChangeAnim(downWalk);
		}
		return;
	}

	auto vec = (_targetPos.ToVector2() - _pos);
	if (vec.Length() <= _speed)
	{
		_pos = _targetPos.ToVector2();
		_isMove = false;
		_fontChangeCnt = FRONT_CHANGE_ITV;
		return;
	}

	if (vec.x > 0)
	{
		_animator->ChangeAnim("RightWalk");
	}
	else
	{
		_animator->ChangeAnim("LeftWalk");
	}
	_pos += vec.Normalized() * static_cast<float>(_speed);

	_animator->Update();
}

void MapSelectCharactor::Draw()
{
	auto size = Size(100, 100);
	_animator->Draw(_camera.GetCameraOffset() + _pos.ToVector2Int() - (size * 0.5f).ToVector2Int(), size);
}

void MapSelectCharactor::SetTargetPos(const Vector2Int& targetPos)
{
	_isMove = true;
	_targetPos = targetPos;
}

bool MapSelectCharactor::GetIsMove() const
{
	return _isMove;
}
