#include <DxLib.h>
#include "BattleCharactor.h"
#include "Application.h"
#include "Animator.h"
#include "Charactor.h"
#include "BattleScene.h"
#include "DxLibUtility.h"
#include "FlyText.h"
#include "FileSystem.h"
#include "DataBase.h"

using namespace std;

BattleCharactor::BattleCharactor(Charactor& charactor)
	: _selfChar(charactor), _size(128, 128)
{
	auto wsize = Application::Instance().GetWindowSize();
	SetStartPos(Vector2());
	_animator = make_shared<Animator>();
	_uiSize = Size(wsize.w / 2, 200);

	_targetChar = nullptr;
}

BattleCharactor::~BattleCharactor()
{
}

void BattleCharactor::Init(const Vector2& startPos, const Dir dir, BattleCharactor* target)
{
	SetStartPos(startPos);
	SetDir(dir);
	SetTargetCharactor(target);

	_attackAnimCnt = -1;
	_attackAnimCntMax = 15;

	_createEffect = false;

	_animHealth = _selfChar.GetStatus().health;
	_animHealthCnt = 0;
}

void BattleCharactor::AnimUpdate()
{
	_animator->Update();
}

void BattleCharactor::AttackUpdate(BattleScene& battleScene)
{
	if (_attackAnimCnt >= 0)
	{
		Vector2 dir = _dir == Dir::left ? Vector2(1, 0) : Vector2(-1, 0);
		float per = _attackAnimCnt / static_cast<float>(_attackAnimCntMax);
		per = 1.0f - abs(0.5f - per) * 2;
		_pos = Lerp(_startPos, _startPos + dir * 50, per);
		if (_attackAnimCnt++ >= _attackAnimCntMax)
		{
			_attackAnimCnt = -1;
		}
		if (!_createEffect && _attackAnimCnt >= _attackAnimCntMax / 2)
		{
			if (_targetChar != nullptr)
			{
				_createEffect = true;

				auto targetCenterPos = _targetChar->GetCenterPos();
				battleScene.GetEffectVec().emplace_back(CreateAttackEffect(targetCenterPos));

				auto selfStatus = _selfChar.GetStatus();
				auto targetStatus = _targetChar->GetSelfCharacotr().GetStatus();

				int damage = selfStatus.GetDamage(targetStatus)
					* Application::Instance().GetDataBase().GetAttributeRate(selfStatus.attribute, targetStatus.attribute);

				char damageText[10];
				sprintf_s(damageText, 10, "%d", damage);
				battleScene.GetEffectVec().emplace_back(make_shared<FlyText>(damageText, targetCenterPos, 60 * 1));
				_targetChar->AddDamage(damage);
			}
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
	auto targetStatus = _targetChar->GetSelfCharacotr().GetStatus();
	auto teamColor = GetTeamColorBattle(_selfChar.GetTeam());
	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin");

	// UIÇÃç∂è„ÇÃç¿ïW
	Vector2Int dirDownPos;
	Vector2Int paramDrawPos;
	Size paramUISize(_uiSize.w / 3, 120);
	Vector2Int nameDrawPos;
	Size nameBoxSize(300, 50);
	Vector2Int sideupPos;

	if (_dir == Dir::left)
	{
		sideupPos = Vector2Int(0, 0);
		dirDownPos = GetDrawPos(Vector2Int(0, wsize.h), _uiSize, Anker::leftdown);
		paramDrawPos = GetDrawPos(Vector2Int(0, wsize.h - _uiSize.h / 2), paramUISize, Anker::leftdown);
		nameDrawPos = GetDrawPos(sideupPos, nameBoxSize, Anker::leftup);
	}
	else
	{
		sideupPos = Vector2Int(wsize.w, 0);
		dirDownPos = GetDrawPos(wsize.ToVector2Int(), _uiSize, Anker::rightdown);
		paramDrawPos = GetDrawPos(wsize.ToVector2Int() - Vector2Int(0, _uiSize.h / 2), paramUISize, Anker::rightdown);
		nameDrawPos = GetDrawPos(sideupPos, nameBoxSize, Anker::rightup);
	}
	// âÊñ â∫UIÇÃóÃàÊï`âÊ
	DrawBox(dirDownPos, dirDownPos + _uiSize.ToVector2Int(), teamColor, true);

	// çUåÇóÕ, ñΩíÜó¶, ïKéE, ÇÃï`âÊ
	Rect _paramRect(paramDrawPos + paramUISize.ToVector2Int() * 0.5, paramUISize);
	_paramRect.Draw(teamColor);
	_paramRect.Draw(0xffffff, false);
	auto drawParam = [&](const int itemNum, const int fontH, const unsigned int color, const char* string, const int num)
	{
		Size strSize;
		int lineCnt;
		GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, fontH, "%d", num);
		Vector2Int paramNumDrawPos = GetDrawPos(Vector2Int(_paramRect.Right(), _paramRect.Top()), strSize, Anker::rightup);
		DrawFormatStringToHandle(paramDrawPos.x + 10, paramDrawPos.y + 40 * itemNum, color, fontH, string);
		DrawFormatStringToHandle(paramNumDrawPos.x - 10, paramNumDrawPos.y + 40 * itemNum, color, fontH, "%d", num);
	};

	int itemNum = 0;
	// çUåÇóÕ
	drawParam(itemNum++, fontHandle, 0xaaaaaa, "ATK", status.GetDamage(targetStatus));
	// ñΩíÜ
	drawParam(itemNum++, fontHandle, 0xaaaaaa, "HIT", 100);
	// ïKéE
	drawParam(itemNum++, fontHandle, 0xaaaaaa, "CRT", 100);

	// HPÇÃêîílï\é¶
	auto startHealth = _selfChar.GetStartStatus().health;
	auto health = _animHealth;
	auto hpDrawPos = dirDownPos + Vector2Int(20, 20) + Vector2Int(0, _uiSize.h / 2);
	DrawFormatStringToHandle(hpDrawPos.x, hpDrawPos.y, 0xaaaaaa, fontHandle, "%d", health);

	Size hpPerDot(5, 30);
	int linePerHp = 40;
	int berCnt = ceil(startHealth / 40.0f);
	auto hpBerDrawPos = GetDrawPos(hpDrawPos + Vector2Int(100, 30), Size(0, berCnt * hpPerDot.h), Anker::leftcenter);
	for (int idx = 0; idx < berCnt; idx++)
	{
		Size startSize = Size(min((startHealth - linePerHp * (berCnt - idx - 1)), linePerHp), 1) * hpPerDot;
		Size currentSize = Size(min((health - linePerHp * (berCnt - idx - 1)), linePerHp), 1) * hpPerDot;
		DrawBox(hpBerDrawPos, hpBerDrawPos + startSize, 0xaaaaaa, true);
		if (currentSize.w > 0)
		{
			DrawBox(hpBerDrawPos, hpBerDrawPos + currentSize, 0x4eb79c, true);
		}
		DrawBox(hpBerDrawPos, hpBerDrawPos + startSize, 0x000000, false);
		hpBerDrawPos.y += hpPerDot.h;
	};

	// ñºëOï\é¶
	Rect nameBox(nameDrawPos + nameBoxSize * 0.5, nameBoxSize);
	nameBox.Draw(teamColor);
	nameBox.Draw(0x000000, false);
	Size strSize;
	int lineCnt;
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, fontHandle, _name.c_str());
	nameDrawPos = GetDrawPos(nameBox.center, strSize, Anker::center) + Vector2Int(5, 5);
	DrawFormatStringToHandle(nameDrawPos.x, nameDrawPos.y, 0xaaaaaa, fontHandle, _name.c_str());
}

void BattleCharactor::StartAttackAnim()
{
	_attackAnimCnt = 0;
	_createEffect = false;
	_animHealth = _selfChar.GetStatus().health;
}

bool BattleCharactor::GetAttackAnimEnd()
{
	return _attackAnimCnt < 0;
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

Charactor& BattleCharactor::GetSelfCharacotr()
{
	return _selfChar;
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

void BattleCharactor::AddDamage(const int damage)
{
	auto status = _selfChar.GetStatus();
	status.health -= damage;
	if (status.health <= 0)
	{
		_selfChar.SetIsDying(true);
	}
	_selfChar.SetStatus(status);
}