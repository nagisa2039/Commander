#include "MapSelectCharactor.h"
#include "Camera.h"
#include "Animator.h"
#include "Team.h"
#include "DataBase.h"
#include "Application.h"
#include "Input.h"
#include "SoundLoader.h"
#include "FileSystem.h"

using namespace std;

namespace
{
	constexpr unsigned int FRONT_CHANGE_ITV = 10;
}

MapSelectCharactor::MapSelectCharactor(Camera& camera, const CharactorType& charactorType, const bool begin)
	: Actor(camera), _begin(begin)
{
	_animator = make_unique<Animator>();
	const Size divSize = Size(32, 32);
	_animator->SetImageHandle(DataBase::Instance().GetCharactorImageHandle(charactorType, Team::player));

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

	_moveSEH = SoundHandle("Resource/Sound/SE/dash.mp3");
}

MapSelectCharactor::~MapSelectCharactor()
{
	SoundL.StopSound(_moveSEH);
}

void MapSelectCharactor::Update(const Input& input)
{
	if (!_isMove)
	{
		const char* downWalk = "DownWalk";
		if (_animator->GetAnimName() != downWalk && --_frontChangeCnt <= 0)
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

		if (_begin)
		{
			SoundL.StopSound(_moveSEH);
		}
		_frontChangeCnt = FRONT_CHANGE_ITV;
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
	if (_begin)
	{
		auto vec = (_targetPos.ToVector2() - _pos);
		if (vec.Length() <= _speed)
		{
			SoundL.StopSound(_moveSEH);
		}
		else
		{
			SoundL.PlaySE(_moveSEH);
		}
	}
}

bool MapSelectCharactor::GetIsMove() const
{
	return _isMove;
}
