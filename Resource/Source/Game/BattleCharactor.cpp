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
#include "CutIn.h"
#include "Tool.h"
#include "Effect/BattleEffect/BattleEffectFactory.h"
#include "ImageLoader.h"
#include "Cast.h"

using namespace std;

namespace
{
	// 攻撃アニメーションカウント最大数
	constexpr int ATTACK_ANIM_CNT_MAX	= 30;
	// 待機カウント最大数
	constexpr int WAIT_CNT_MAX			= 10;
	//HPアニメーション間隔
	constexpr int HP_ANIM_ITV			= 3;
}

BattleCharactor::BattleCharactor(Charactor& charactor, const int imageHandle, Camera& camera)
	: _selfChar(charactor), _size{ 128, 128 }, _camera(camera)
{
	auto wsize = Application::Instance().GetWindowSize();
	SetStartPos(Vector2{0,0});
	_animator = make_shared<Animator>();

	_targetChar = nullptr;

	_attackAnimX = make_unique<Track<int>>();
	_attackAnimX->AddKey(0, 0);
	_attackAnimX->AddKey(ATTACK_ANIM_CNT_MAX / 2, 50);
	_attackAnimX->AddKey(ATTACK_ANIM_CNT_MAX, 0);

	_waitT = make_unique<Track<int>>();
	_waitT->AddKey(0, 0);
	_waitT->AddKey(WAIT_CNT_MAX, 0);

	_attackEffect = nullptr;

	_givenDamage = 0;

	_hpDotMaskH = ImageL.LoadMask("Resource/Image/Battle/hpDotMask.bmp");

	_gaveDamageType = damageType::none;
	_receiveDamageType = damageType::none;
	const Size divSize = Size{ 32, 32 };
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

	animRectVec.emplace_back(Rect{ Vector2Int{16, 16 + 32 * 1}, divSize });
	animRectVec.emplace_back(Rect{ Vector2Int{ 16 + divSize.w * 2, 16 + 32 * 1 }, divSize });
	_animator->AddAnim("LeftWalk", animRectVec, 30, true);
	animRectVec.clear();

	animRectVec.emplace_back(Rect{ Vector2Int{ 16, 16 + 32 * 2}, divSize });
	animRectVec.emplace_back(Rect{ Vector2Int{ 16 + divSize.w * 2, 16 + 32 * 2}, divSize });
	_animator->AddAnim("RightWalk", animRectVec, 30, true);


	_waitNextUpdater = &BattleCharactor::NormalUpdate;

	_attackEffectFuncs[Uint64(damageType::none)] = [this](BattleScene& battleScene, const Vector2Int center)
	{
		auto& dataBase = DataBase::Instance();
		auto effect = dataBase.GetBattleEffectFactory().
			CreateBattleEffect(BattleEffectType::miss, *this, *_targetChar, battleScene.GetEffectVec(), true, _camera);
		battleScene.GetEffectVec().emplace_back(effect);
	};

	_attackEffectFuncs[Uint64(damageType::damage)] = [this](BattleScene& battleScene, const Vector2Int center)
	{
		auto& dataBase = DataBase::Instance();
		auto type = dataBase.GetWeaponData(_selfChar.GetStatus().weaponId).effectType;
		_attackEffect = dataBase.GetBattleEffectFactory().
			CreateBattleEffect(type, *this, *_targetChar, battleScene.GetEffectVec(), false, _camera);
		battleScene.GetEffectVec().emplace_back(_attackEffect);
	};

	_attackEffectFuncs[Uint64(damageType::critical)] = [this](BattleScene& battleScene, const Vector2Int center)
	{
		auto& dataBase = DataBase::Instance();
		auto type = dataBase.GetWeaponData(_selfChar.GetStatus().weaponId).effectType;
		_attackEffect = dataBase.GetBattleEffectFactory().
			CreateBattleEffect(type, *this, *_targetChar, battleScene.GetEffectVec(), true, _camera);
		battleScene.GetEffectVec().emplace_back(_attackEffect);
	};
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

void BattleCharactor::Update(BattleScene& battleScene)
{
	(this->*_updater)(battleScene);
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
		_waitT->Update();
		return;
	}

	if (_animHealthCnt++ % HP_ANIM_ITV == 0)
	{
		int sub = statusHp - _animHealth;
		_animHealth += sub /abs(sub);
		if (statusHp == _animHealth)
		{
			_receiveDamageType = damageType::none;
		}
		SoundL.PlaySE("Resource/Sound/SE/hp_se.mp3");
	}
}

void BattleCharactor::UIDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	auto teamColor  = GetTeamColorBattle(_selfChar.GetTeam());
	auto& fileSystem = FileSystem::Instance();

	auto fontHandle = fileSystem.GetFontHandle("choplin40edge");
	
	// UIの左上の座標
	Rect windowRect{ Vector2Int{0,0}, Size{wsize.w / 2, 200} };
	Rect paramWindowRect{ Vector2Int{ 0,0}, Size{windowRect.size.w / 3, 120}};
	Rect nameWindowRect{ Vector2Int{ 0, 0}, Size{300,50} };
	Rect weaponNameRect{ Vector2Int{ 0, 0}, Size{windowRect.size.w / 3 * 2, 100}};

	const char* teamString = _selfChar.GetTeam() == Team::player ? "player" : "enemy";

	if (_dir == Dir::left)
	{
		windowRect.center = Vector2Int{ wsize.w / 2 - windowRect.size.w/2, wsize.h - windowRect.size.h /2 };
		paramWindowRect.center = Vector2Int{ paramWindowRect.size.w/2, windowRect.center.y - paramWindowRect.size.h/2 };
		nameWindowRect.center = nameWindowRect.size.ToVector2Int() * 0.5f;
		weaponNameRect.center = Vector2Int{ wsize.w / 2 - weaponNameRect.size.w / 2, windowRect.center.y - weaponNameRect.size.h/2 };
	}
	else
	{
		windowRect.center = Vector2Int{ wsize.w / 2 + windowRect.size.w / 2, wsize.h - windowRect.size.h / 2 };
		paramWindowRect.center = Vector2Int{ wsize.w - paramWindowRect.size.w / 2, windowRect.center.y - paramWindowRect.size.h / 2 };
		nameWindowRect.center = Vector2Int{ wsize.w, 0 } + Vector2Int{ -nameWindowRect.size.w, nameWindowRect.size.h } *0.5f;
		weaponNameRect.center = Vector2Int{ wsize.w / 2 + weaponNameRect.size.w / 2, windowRect.center.y - weaponNameRect.size.h / 2 };
	}

	// 画面下UIの領域描画
	char path[_MAX_PATH];
	sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleWindow_%s.png", teamString);
	windowRect.DrawGraph(fileSystem.GetImageHandle(path));

	// パラメータ表示
	DrawParameter(teamString, fileSystem, paramWindowRect);

	// HP表示
	DrawHP(windowRect, fontHandle);

	// 武器名の表示
	DrawWeaponName(fileSystem, weaponNameRect);

	// 名前表示
	DrawName(teamString, nameWindowRect, fileSystem, fontHandle);
}

void BattleCharactor::WaitUpdate(BattleScene& battleScene)
{
	_waitT->Update();
	if (_waitT->GetEnd())
	{
		_updater = _waitNextUpdater;
	}
}

void BattleCharactor::NormalUpdate(BattleScene& battleScene)
{
	_attackAnimX->Update();
	auto dir = _dir == Dir::left ? 1 : -1;
	_pos = _startPos + Vector2{static_cast<float>(_attackAnimX->GetValue() * dir), 0};

	if (_attackAnimX->GetFrame() == ATTACK_ANIM_CNT_MAX/2)
	{
		_attackEffectFuncs[Uint64(_gaveDamageType)](battleScene, _targetChar->GetCenterPos());
	}
}

void BattleCharactor::CutInUpdate(BattleScene& battleScene)
{
}

void BattleCharactor::WaitStart(void(BattleCharactor::* nextUpdate)(BattleScene&))
{
	_updater = &BattleCharactor::WaitUpdate;
	_waitNextUpdater = nextUpdate;
}

void BattleCharactor::DrawName(const char* teamString, Rect& nameWindowRect, FileSystem& fileSystem, int fontHandle)
{
	char path[_MAX_PATH];
	sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleWindow_%s.png", teamString);
	sprintf_s(path, _MAX_PATH, "Resource/Image/Battle/battleNameWindow_%s.png", teamString);
	nameWindowRect.DrawGraph(fileSystem.GetImageHandle(path));
	DrawStringToHandle(nameWindowRect.center, Anker::center, 0xffffff, fontHandle, _selfChar.GetName().c_str());
}

void BattleCharactor::DrawParameter(const char* teamString, FileSystem& fileSystem, Rect& paramWindowRect)
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
		DrawStringToHandle(Vector2Int{ paramWindowRect.Left() + 10, drawY }, Anker::leftcenter, color, fontH, string);
		// 攻撃できるか
		if (_selfChar.GetAttackRange().Hit(distance) && (!battleSelfStatus.CheckHeal() || _dir == Dir::left))
		{
			char numStr[10];
			sprintf_s(numStr, 10, "%d", num);
			DrawStringToHandle(Vector2Int{ paramWindowRect.Right() - 10, drawY }, Anker::rightcenter, color, fontH, numStr);
		}
		else
		{
			DrawStringToHandle(Vector2Int{ paramWindowRect.Right() - 10, drawY }, Anker::rightcenter, color, fontH, "--");
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
	Vector2Int hpBerOffset{ 0, 0 };
	Vector2Int hpNumOffset{0, 0};
	if (_receiveDamageType == damageType::damage || _receiveDamageType == damageType::critical)
	{
		int range = _receiveDamageType == damageType::damage ? 3 : 8;
		hpBerOffset = Vector2Int{ rand() % range, rand() % range };
		hpNumOffset = Vector2Int{ rand() % range, rand() % range };
	}

	// HPの数値表示
	auto startHealth = _selfChar.GetStartStatus().health;
	auto health = _animHealth;
	auto hpDrawPos = Vector2Int{ windowRect.Left(), windowRect.center.y } + Vector2Int{ 20, 20 };
	DrawFormatStringToHandle(hpDrawPos.x + hpNumOffset.x, hpDrawPos.y + hpNumOffset.y, 0xffffff, fontHandle, "%d", health);

	Size hpPerDot{ 10, 30 };
	int linePerHp = 40;
	int berCnt = static_cast<int>(ceil(startHealth / 40.0f));


	auto hpBerDrawPos = GetDrawPos(hpDrawPos + Vector2Int{ 100, 30 }, Size{ 0, berCnt * hpPerDot.h }, Anker::leftcenter) + hpBerOffset;
	for (int idx = 0; idx < berCnt; idx++)
	{
		Size startSize = Size{ min((startHealth - linePerHp * (berCnt - idx - 1)), linePerHp), 1 } *hpPerDot;
		Size currentSize = Size{ min((health - linePerHp * (berCnt - idx - 1)), linePerHp), 1 } *hpPerDot;
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
	const char* str = DataBase::Instance().GetWeaponData(GetCharacotr().GetStartStatus().weaponId).name.c_str();
	DrawStringToHandle(weaponNameRect.center, Anker::center, 0xffffff, choplin30, str);
}

void BattleCharactor::StartAttackAnim(BattleScene& battleScene)
{
	_animHealth = _selfChar.GetStatus().health;
	_waitT->Reset();
	_attackAnimX->Reset();

	assert(_targetChar);

	auto selfBattleStatus = _selfChar.GetBattleStatus();
	auto targetBattleStatus = _targetChar->GetCharacotr().GetBattleStatus();

	_gaveDamageType = damageType::damage;
	bool heal = selfBattleStatus.CheckHeal();
	if (heal)
	{
		WaitStart(&BattleCharactor::NormalUpdate);
		return;
	}

	// 命中判定
	if (Hit(selfBattleStatus.GetHit(targetBattleStatus)))
	{
		// 会心判定
		if (Hit(selfBattleStatus.GetCritical(targetBattleStatus)))
		{
			_gaveDamageType = damageType::critical;
			_updater = &BattleCharactor::CutInUpdate;
			battleScene.SetCutIn(make_unique<CutIn>(
				_selfChar.GetCharactorType(), _selfChar.GetTeam(), _dir, [this]() 
				{WaitStart(&BattleCharactor::NormalUpdate);}));
			return;
		}
		WaitStart(&BattleCharactor::NormalUpdate);
	}
	else
	{
		_gaveDamageType = damageType::none;
		WaitStart(&BattleCharactor::NormalUpdate);
	}
}
bool BattleCharactor::GetAttackAnimEnd()
{
	return _attackAnimX->GetEnd()
		&& (_attackEffect == nullptr || _attackEffect->GetDelete());
}

void BattleCharactor::StartHPAnim()
{
	_waitT->Reset();
	_animHealthCnt = 0;
}

bool BattleCharactor::GetHPAnimEnd()
{
	return _waitT->GetEnd();
}

Size BattleCharactor::GetSize() const
{
	return _size;
}

Vector2Int BattleCharactor::GetCenterPos() const
{
	return Vector2Int{_pos.ToVector2Int() - Vector2Int{ 0, _size.h / 2 }};
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
	return _receiveDamageType;
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

void BattleCharactor::SetReceiveDamageType(const damageType dt)
{
	_receiveDamageType = dt;
}

void BattleCharactor::SetGivenDamage(const unsigned int value)
{
	_givenDamage = value;
}