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
#include "SoundLoader.h"

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
	_damageType = damageType::none;
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
	_pos = _startPos + Vector2(static_cast<float>(_attackAnimX->GetValue() * dir), 0);

	if (_attackAnimX->GetFrame() == 15)
	{
		if (_targetChar != nullptr)
		{
			// 攻撃
			auto selfBattleStatus = _selfChar.GetBattleStatus();
			auto targetBattleStatus = _targetChar->GetCharacotr().GetBattleStatus();

			auto targetCenterPos = _targetChar->GetCenterPos();

			// 命中判定
			if (!selfBattleStatus.CheckHeal() && selfBattleStatus.GetHit(targetBattleStatus) <= rand() % 100)
			{
				// 外れ
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

		auto& soundLoader = Application::Instance().GetFileSystem().GetSoundLoader();
		soundLoader.PlaySE("Resource/Sound/SE/select01.mp3");
	}
}

void BattleCharactor::UIDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	auto teamColor  = GetTeamColorBattle(_selfChar.GetTeam());
	auto& fileSystem = Application::Instance().GetFileSystem();

	auto fontHandle = fileSystem.GetFontHandle("choplin40edge");
	
	// UIの左上の座標
	Rect windowRect(Vector2Int(0,0), Size(wsize.w / 2, 200));
	Rect paramWindowRect(Vector2Int(0,0), Size(windowRect.size.w / 3, 120));
	Rect nameWindowRect(Vector2Int(0, 0), Size(300,50));
	Rect weaponNameRect(Vector2Int(0, 0), Size(windowRect.size.w / 3 * 2, 100));

	const char* teamString = _selfChar.GetTeam() == Team::player ? "player" : "enemy";

	if (_dir == Dir::left)
	{
		windowRect.center = Vector2Int(wsize.w / 2 - windowRect.size.w/2, wsize.h - windowRect.size.h /2);
		paramWindowRect.center = Vector2Int(paramWindowRect.size.w/2, windowRect.center.y - paramWindowRect.size.h/2);
		nameWindowRect.center = nameWindowRect.size.ToVector2Int() * 0.5f;
		weaponNameRect.center = Vector2Int(wsize.w / 2 - weaponNameRect.size.w / 2, windowRect.center.y - weaponNameRect.size.h/2);
	}
	else
	{
		windowRect.center = Vector2Int(wsize.w / 2 + windowRect.size.w / 2, wsize.h - windowRect.size.h / 2);
		paramWindowRect.center = Vector2Int(wsize.w - paramWindowRect.size.w / 2, windowRect.center.y - paramWindowRect.size.h / 2);
		nameWindowRect.center = Vector2Int(wsize.w, 0) + Vector2Int(-nameWindowRect.size.w, nameWindowRect.size.h) * 0.5f;
		weaponNameRect.center = Vector2Int(wsize.w / 2 + weaponNameRect.size.w / 2, windowRect.center.y - weaponNameRect.size.h / 2);
	}

	// 画面下UIの領域描画
	char path[_MAX_PATH];
	sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleWindow_%s.png", teamString);
	windowRect.DrawGraph(fileSystem.GetImageHandle(path));

	// パラメータ表示
	DrawParameter(teamString, windowRect, fileSystem, paramWindowRect);

	// HP表示
	DrawHP(windowRect, fontHandle);

	// 武器名の表示
	DrawWeaponName(fileSystem, weaponNameRect);

	// 名前表示
	DrawName(teamString, nameWindowRect, fileSystem, fontHandle);
}

void BattleCharactor::DrawName(const char* teamString, Rect& nameWindowRect, FileSystem& fileSystem, int fontHandle)
{
	char path[_MAX_PATH];
	sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleWindow_%s.png", teamString);
	sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleNameWindow_%s.png", teamString);
	nameWindowRect.DrawGraph(fileSystem.GetImageHandle(path));
	DrawStringToHandle(nameWindowRect.center, Anker::center, 0xffffff, fontHandle, _selfChar.GetName().c_str());
}

void BattleCharactor::DrawParameter(const char* teamString, Rect& windowRect, FileSystem& fileSystem, Rect& paramWindowRect)
{
	char path[_MAX_PATH];
	sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleParamWindow_%s.png", teamString);

	auto battleSelfStatus = _selfChar.GetBattleStatus();
	auto targetBattleStatus = _targetChar->GetCharacotr().GetBattleStatus();
	Vector2Int mapPosSub(_targetChar->GetCharacotr().GetMapPos() - _selfChar.GetMapPos());
	unsigned int distance = abs(mapPosSub.x) + abs(mapPosSub.y);

	// 攻撃力, 命中率, 必殺, の描画
	paramWindowRect.DrawGraph(fileSystem.GetImageHandle(path));

	const int ITEM_MAX = 3;
	auto drawParam = [&](const int itemNum, const int fontH, const unsigned int color, const char* string, const int num)
	{
		int drawY = paramWindowRect.center.y + (itemNum - (ITEM_MAX - 2)) * 40;
		DrawStringToHandle(Vector2Int(paramWindowRect.Left() + 10, drawY), Anker::leftcenter, color, fontH, string);
		// 攻撃できるか
		if (_selfChar.GetAttackRange().Hit(distance) && (!battleSelfStatus.CheckHeal() || _dir == Dir::left))
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
	// 攻撃力
	const char* name = "";
	int num = 0;

	if (battleSelfStatus.CheckHeal())
	{
		name = "RCV";
		num = battleSelfStatus.GetRecover();
	}
	else
	{
		name = "ATK";
		num = battleSelfStatus.GetDamage(targetBattleStatus);
	}
	auto paramH = fileSystem.GetFontHandle("choplin30edge");
	drawParam(itemNum++, paramH, 0xffffff, name, num);
	// 命中
	drawParam(itemNum++, paramH, 0xffffff, "HIT", battleSelfStatus.GetHit(targetBattleStatus));
	// 必殺
	drawParam(itemNum++, paramH, 0xffffff, "CRT", battleSelfStatus.GetCritical(targetBattleStatus));
}

void BattleCharactor::DrawHP(Rect& windowRect, int fontHandle)
{
	// 被ダメージ時の揺れ用オフセット
	Vector2Int hpBerOffset(0,0);
	Vector2Int hpNumOffset(0,0);
	if (_damageType == damageType::damage || _damageType == damageType::critical)
	{
		int range = _damageType == damageType::damage ? 3 : 8;
		hpBerOffset = Vector2Int(rand() % range, rand() % range);
		hpNumOffset = Vector2Int(rand() % range, rand() % range);
	}

	// HPの数値表示
	auto startHealth = _selfChar.GetStartStatus().health;
	auto health = _animHealth;
	auto hpDrawPos = Vector2Int(windowRect.Left(), windowRect.center.y) + Vector2Int(20, 20);
	DrawFormatStringToHandle(hpDrawPos.x + hpNumOffset.x, hpDrawPos.y + hpNumOffset.y, 0xffffff, fontHandle, "%d", health);

	Size hpPerDot(10, 30);
	int linePerHp = 40;
	int berCnt = static_cast<int>(ceil(startHealth / 40.0f));


	auto hpBerDrawPos = GetDrawPos(hpDrawPos + Vector2Int(100, 30), Size(0, berCnt * hpPerDot.h), Anker::leftcenter) + hpBerOffset;
	for (int idx = 0; idx < berCnt; idx++)
	{
		Size startSize = Size(min((startHealth - linePerHp * (berCnt - idx - 1)), linePerHp), 1) * hpPerDot;
		Size currentSize = Size(min((health - linePerHp * (berCnt - idx - 1)), linePerHp), 1) * hpPerDot;
		DrawBox(hpBerDrawPos, hpBerDrawPos + startSize, 0x888888, true);
		if (currentSize.w > 0)
		{
			DrawBox(hpBerDrawPos, hpBerDrawPos + currentSize, 0x4eb79c, true);
		}
		CreateMaskScreen();
		DrawFillMask(hpBerDrawPos.x, hpBerDrawPos.y, hpBerDrawPos.x + startSize.w, hpBerDrawPos.y + startSize.h, _hpDotMaskH);
		DrawBox(hpBerDrawPos, hpBerDrawPos + startSize, 0x000000, true);
		DeleteMaskScreen();
		hpBerDrawPos.y += hpPerDot.h;
	};
}

void BattleCharactor::DrawWeaponName(FileSystem& fileSystem, Rect& weaponNameRect)
{
	auto choplin30 = fileSystem.GetFontHandle("choplin30edge");
	const char* str = Application::Instance().GetDataBase().GetWeaponData(GetCharacotr().GetStartStatus().weaponId).name.c_str();
	DrawStringToHandle(weaponNameRect.center, Anker::center, 0xffffff, choplin30, str);
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

BattleCharactor::damageType BattleCharactor::GetDamageType() const
{
	return _damageType;
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

void BattleCharactor::SetDamageType(const damageType dt)
{
	_damageType = dt;
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
	return make_shared<PopupText>("MISS!", effectPos, _camera, false);
}