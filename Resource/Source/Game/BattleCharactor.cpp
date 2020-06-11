#include <DxLib.h>
#include "BattleCharactor.h"
#include "Application.h"
#include "Animator.h"
#include "Charactor.h"
#include "BattleScene.h"
#include "DxLibUtility.h"
#include "FileSystem.h"
#include "DataBase.h"
#include "Effect/MissEffect.h"
#include "Effect/FlyText.h"

using namespace std;

int BattleCharactor::_hpDotMaskH = -1;

BattleCharactor::BattleCharactor(Charactor& charactor, const int imageHandle, Camera& camera)
	: _selfChar(charactor), _size(128, 128), _camera(camera)
{
	auto wsize = Application::Instance().GetWindowSize();
	SetStartPos(Vector2());
	_animator = make_shared<Animator>();

	_targetChar = nullptr;

	_attackAnimX = make_unique<Track<int>>();
	_attackAnimX->AddKey(0, 0);
	_attackAnimX->AddKey(15, 50);
	_attackAnimX->AddKey(30, 0);

	_attackEffect = nullptr;

	_givenDamage = 0;

	if (_hpDotMaskH == -1)
	{
		_hpDotMaskH = LoadMask("Resource/Image/Battle/hpDotMask.png");
	}
}

BattleCharactor::~BattleCharactor()
{
}

void BattleCharactor::Init(const Vector2& startPos, const Dir dir, BattleCharactor* target)
{
	SetStartPos(startPos);
	SetDir(dir);
	SetTargetCharactor(target);

	_animHealth = _selfChar.GetStatus().health;
	_animHealthCnt = 0;

	_givenDamage = 0;
}

void BattleCharactor::AnimUpdate()
{
	_animator->Update();
}

void BattleCharactor::AttackUpdate(BattleScene& battleScene)
{
	_attackAnimX->Update();
	auto dir = _dir == Dir::left ? 1 : -1;
	_pos = _startPos + Vector2(_attackAnimX->GetValue() * dir, 0);

	if (_attackAnimX->GetFrame() == 15)
	{
		if (_targetChar != nullptr)
		{
			// çUåÇ
			auto selfStatus = _selfChar.GetStatus();
			auto targetStatus = _targetChar->GetCharacotr().GetStatus();

			auto targetCenterPos = _targetChar->GetCenterPos();

			// ñΩíÜîªíË
			if (!selfStatus.heal && selfStatus.GetHit(targetStatus) <= rand() % 100)
			{
				// äOÇÍ
				battleScene.GetEffectVec().emplace_back(CreateMissEffect(targetCenterPos));
				return;
			}

			_attackEffect = CreateAttackEffect(battleScene.GetEffectVec());
			battleScene.GetEffectVec().emplace_back(_attackEffect);
		}
	}
}

void BattleCharactor::Draw()
{
	_animator->Draw(GetDrawPos(_pos.ToVector2Int(), _size, Anker::centerdown), _size);
	UIDraw();
}

void BattleCharactor::UIAnimUpdate()
{
	uint8_t statusHp = _selfChar.GetStatus().health;
	if (statusHp == _animHealth)
	{
		return;
	}

	if (_animHealthCnt++ % 3 == 0)
	{
		_animHealth += _animHealth > statusHp ? -1 : 1;
	}
}

void BattleCharactor::UIDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	auto status = _selfChar.GetStatus();
	auto targetStatus = _targetChar->GetCharacotr().GetStatus();
	auto teamColor = GetTeamColorBattle(_selfChar.GetTeam());
	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin40");

	Vector2Int mapPosSub(_targetChar->GetCharacotr().GetMapPos() - _selfChar.GetMapPos());
	unsigned int distance = abs(mapPosSub.x) + abs(mapPosSub.y);
	
	// UIÇÃç∂è„ÇÃç¿ïW
	Rect windowRect(Vector2Int(0,0), Size(wsize.w / 2, 200));
	Rect paramWindowRect(Vector2Int(0,0), Size(windowRect.size.w / 3, 120));
	Rect nameWindowRect(Vector2Int(0, 0), Size(300,50));

	const char* teamString = _selfChar.GetTeam() == Team::player ? "player" : "enemy";

	if (_dir == Dir::left)
	{
		windowRect.center = Vector2Int(wsize.w / 2 - windowRect.size.w/2, wsize.h - windowRect.size.h /2);
		paramWindowRect.center = Vector2Int(paramWindowRect.size.w/2, windowRect.center.y - paramWindowRect.size.h/2);
		nameWindowRect.center = nameWindowRect.size.ToVector2Int() * 0.5f;
	}
	else
	{
		windowRect.center = Vector2Int(wsize.w / 2 + windowRect.size.w / 2, wsize.h - windowRect.size.h / 2);
		paramWindowRect.center = Vector2Int(wsize.w - paramWindowRect.size.w / 2, windowRect.center.y - paramWindowRect.size.h / 2);
		nameWindowRect.center = Vector2Int(wsize.w, 0) + Vector2Int(-nameWindowRect.size.w, nameWindowRect.size.h) * 0.5f;
	}

	// âÊñ â∫UIÇÃóÃàÊï`âÊ
	char path[_MAX_PATH];
	sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleWindow_%s.png", teamString);
	windowRect.DrawGraph(Application::Instance().GetFileSystem()->GetImageHandle(path));

	{
		// çUåÇóÕ, ñΩíÜó¶, ïKéE, ÇÃï`âÊ
		sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleParamWindow_%s.png", teamString);
		paramWindowRect.DrawGraph(Application::Instance().GetFileSystem()->GetImageHandle(path));

		const int ITEM_MAX = 3;
		auto drawParam = [&](const int itemNum, const int fontH, const unsigned int color, const char* string, const int num)
		{
			int drawY = paramWindowRect.center.y + (itemNum - (ITEM_MAX - 2)) * 40;
			DrawStringToHandle(Vector2Int(paramWindowRect.Left() + 10, drawY), Anker::leftcenter, color, fontH, string);
			// çUåÇÇ≈Ç´ÇÈÇ©
			if (_selfChar.GetAttackRange().Hit(distance) && (!status.heal || _dir == Dir::left))
			{
				char numStr[10];
				sprintf_s(numStr, 10, "%d", num);
				DrawStringToHandle(Vector2Int(paramWindowRect.Right() - 10, drawY), Anker::rightcenter, color, fontH, numStr);
			}
			else
			{
				DrawStringToHandle(Vector2Int(paramWindowRect.Right() - 10, drawY), Anker::rightcenter, color, fontH, "--");
			}
		};

		int itemNum = 0;
		// çUåÇóÕ
		const char* name = "";
		int num = 0;
		if (status.heal)
		{
			name = "RCV";
			num = status.GetRecover();
		}
		else
		{
			name = "ATK";
			num = status.GetDamage(targetStatus);
		}
		auto paramH = Application::Instance().GetFileSystem()->GetFontHandle("choplin30");
		drawParam(itemNum++, paramH, 0xffffff, name, num);
		// ñΩíÜ
		drawParam(itemNum++, paramH, 0xffffff, "HIT", status.GetHit(targetStatus));
		// ïKéE
		drawParam(itemNum++, paramH, 0xffffff, "CRT", status.GetCritical(targetStatus));
	}

	{
		// HPÇÃêîílï\é¶
		auto startHealth = _selfChar.GetStartStatus().health;
		auto health = _animHealth;
		auto hpDrawPos = Vector2Int(windowRect.Left(), windowRect.center.y) + Vector2Int(20, 20);
		DrawFormatStringToHandle(hpDrawPos.x, hpDrawPos.y, 0xffffff, fontHandle, "%d", health);

		Size hpPerDot(10, 30);
		int linePerHp = 40;
		int berCnt = ceil(startHealth / 40.0f);

		auto hpBerDrawPos = GetDrawPos(hpDrawPos + Vector2Int(100, 30), Size(0, berCnt * hpPerDot.h), Anker::leftcenter);
		for (int idx = 0; idx < berCnt; idx++)
		{
			Size startSize = Size(min((startHealth - linePerHp * (berCnt - idx - 1)), linePerHp), 1) * hpPerDot;
			Size currentSize = Size(min((health - linePerHp * (berCnt - idx - 1)), linePerHp), 1) * hpPerDot;
			DrawBox(hpBerDrawPos, hpBerDrawPos + startSize, 0x888888, true);
			if (currentSize.w > 0)
			{
				DrawBox(hpBerDrawPos, hpBerDrawPos + currentSize, 0x000000, true);
				CreateMaskScreen();
				DrawFillMask(hpBerDrawPos.x, hpBerDrawPos.y, hpBerDrawPos.x + currentSize.w, hpBerDrawPos.y + currentSize.h, _hpDotMaskH);
				DrawBox(hpBerDrawPos, hpBerDrawPos + currentSize, 0x4eb79c, true);
				DeleteMaskScreen();
			}
			DrawBox(hpBerDrawPos, hpBerDrawPos + startSize, 0x000000, false);
			hpBerDrawPos.y += hpPerDot.h;
		};
	}

	{
		// ñºëOï\é¶
		sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleNameWindow_%s.png", teamString);
		nameWindowRect.DrawGraph(Application::Instance().GetFileSystem()->GetImageHandle(path));
		DrawStringToHandle(nameWindowRect.center, Anker::center, 0xffffff, fontHandle, _selfChar.GetName().c_str());
	}
}

void BattleCharactor::StartAttackAnim()
{
	_animHealth = _selfChar.GetStatus().health;
	_attackAnimX->Reset();
}

bool BattleCharactor::GetAttackAnimEnd()
{
	return _attackAnimX->GetEnd() && (_attackEffect == nullptr || _attackEffect->GetDelete());
}

void BattleCharactor::StartHPAnim()
{
	_animHealthCnt = 0;
}

bool BattleCharactor::GetHPAnimEnd()
{
	return _animHealth == _selfChar.GetStatus().health;
}

Size BattleCharactor::GetSize() const
{
	return _size;
}

Vector2Int BattleCharactor::GetCenterPos() const
{
	return Vector2Int(_pos.ToVector2Int() - Vector2Int(0, _size.h / 2));
}

Vector2Int BattleCharactor::GetPos() const
{
	return _pos.ToVector2Int();
}

Vector2Int BattleCharactor::GetStartPos() const
{
	return _startPos.ToVector2Int();
}

Charactor& BattleCharactor::GetCharacotr()
{
	return _selfChar;
}

const unsigned int& BattleCharactor::GetGivenDamage() const
{
	return _givenDamage;
}

BattleCharactor* BattleCharactor::GetTargetBattleCharactor()
{
	return _targetChar;
}

void BattleCharactor::SetStartPos(const Vector2& startPos)
{
	_startPos = startPos;
	_pos = startPos;
}

void BattleCharactor::SetDir(const Dir dir)
{
	_dir = dir;

	if (dir == Dir::right)
	{
		_animator->ChangeAnim("LeftWalk");
	}
	else
	{
		_animator->ChangeAnim("RightWalk");
	}
}

void BattleCharactor::SetTargetCharactor(BattleCharactor* target)
{
	_targetChar = target;
}

void BattleCharactor::SetGivenDamage(const unsigned int value)
{
	_givenDamage = value;
}

void BattleCharactor::AddGivenDamage(const unsigned int value)
{
	_givenDamage += value;
}

std::shared_ptr<Effect> BattleCharactor::CreateMissEffect(const Vector2Int& effectPos)
{
	return make_shared<MissEffect>(effectPos, _camera);
}