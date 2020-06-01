#include "EditCursor.h"
#include "../Utility/Input.h"
#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"
#include "Application.h"
#include "FileSystem.h"
#include <DxLib.h>
#include <algorithm>

using namespace std;

EditCursor::EditCursor(MapCtrl& mapCtrl, Camera& camera): MapCursor(mapCtrl, camera)
{
	_mapPos = Vector2Int(0, 0);
	_pos = Vector2(0, 0);
	_mapChip = Map_Chip::forest;
	_charactorChipInf.type = static_cast<CharactorType>(0);
	_charactorChipInf.level = 5;
	_charactorChipInf.team = Team::player;
	_charactorChipInf.mapPos = _mapPos;
	_charactorChipInf.groupNum = 0;
	_charactorChipInf.active = false;

	_animCnt = 0;

	_uniqueUpdater = &EditCursor::MapEidtUpdate;
	_uniqueDrawer = &EditCursor::MapEditDraw;
}

EditCursor::~EditCursor()
{
}

void EditCursor::MapEidtUpdate(const Input& input)
{
	auto ChipChange = [&](const char key, const int move)
	{
		if (input.GetButtonDown(key))
		{
			_mapChip = static_cast<Map_Chip>(static_cast<int>(_mapChip) + move);
			if (_mapChip < Map_Chip::none)
			{
				_mapChip = static_cast<Map_Chip>(static_cast<int>(Map_Chip::max) - 1);
			}
			if (_mapChip >= Map_Chip::max)
			{
				_mapChip = Map_Chip::none;
			}
			_animCnt = 0;
		}
	};

	ChipChange(KEY_INPUT_Z, 1);
	ChipChange(KEY_INPUT_X, -1);

	if (input.GetButtonDown(0, "space"))
	{
		_mapCtrl.SetMapChip(_mapPos, _mapChip);
	}

	if (input.GetButtonDown(0, "change"))
	{
		_uniqueUpdater = &EditCursor::CharactorEditUpdate;
		_uniqueDrawer = &EditCursor::CharactorEditDraw;
	}
}

void EditCursor::CharactorEditUpdate(const Input& input)
{
	auto CharactorChange = [&](const char key, const int move)
	{
		if (input.GetButtonDown(key))
		{
			_charactorChipInf.type = static_cast<CharactorType>(static_cast<int>(_charactorChipInf.type) + move);
			if (static_cast<int>(_charactorChipInf.type) < 0)
			{
				_charactorChipInf.type = static_cast<CharactorType>(static_cast<int>(CharactorType::max));
			}
			if (_charactorChipInf.type > CharactorType::max)
			{
				_charactorChipInf.type = static_cast<CharactorType>(0);
			}
			_animCnt = 0;
		}
	};

	if (_charactorChipInf.team != Team::player)
	{
		CharactorChange(KEY_INPUT_Z, 1);
		CharactorChange(KEY_INPUT_X, -1);
	}
	else
	{
		if (input.GetButtonDown(KEY_INPUT_Z) || input.GetButtonDown(KEY_INPUT_X))
		{
			_charactorChipInf.type = _charactorChipInf.type == CharactorType::max ? CharactorType::swordman : CharactorType::max;
		}
	}

	if (input.GetButtonDown(0, "change"))
	{
		_uniqueUpdater = &EditCursor::MapEidtUpdate;
		_uniqueDrawer = &EditCursor::MapEditDraw;
		return;
	}

	if (input.GetButtonDown(0, "team"))
	{
		_charactorChipInf.team = _charactorChipInf.team == Team::player ? Team::enemy : Team::player;
	}

	if (input.GetButtonDown(0, "+"))
	{
		_charactorChipInf.level = min(max(0, _charactorChipInf.level + 1), 100);
	}
	if (input.GetButtonDown(0, "-"))
	{
		_charactorChipInf.level = min(max(0, _charactorChipInf.level - 1), 100);
	}

	if (input.GetButtonDown(KEY_INPUT_I))
	{
		_charactorChipInf.groupNum = min(max(0, _charactorChipInf.groupNum + 1), 99);
	}
	if (input.GetButtonDown(KEY_INPUT_U))
	{
		_charactorChipInf.groupNum = min(max(0, _charactorChipInf.groupNum + 1), 99);
	}

	_charactorChipInf.mapPos = _mapPos;

	if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		auto charChipInf = _charactorChipInf;
		charChipInf.groupNum += _charactorChipInf.team == Team::enemy ? 100 : 0;
		_mapCtrl.SetCharactorChip(charChipInf);
	}
	if (input.GetButtonDown(KEY_INPUT_M))
	{
		_charactorChipInf.active = !_charactorChipInf.active;
	}
}

void EditCursor::Update(const Input& input)
{
	(this->*_uniqueUpdater)(input);

	CursorMove(input);

	_animCnt += 5;
	_pos = (_mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
}

void EditCursor::MapEditDraw()
{
	auto offset = _camera.GetCameraOffset();
	int alpha = abs(255 - (_animCnt % 512));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	if (_mapChip != Map_Chip::none)
	{
		_mapCtrl.DrawMapChip(_mapPos, _mapChip, offset);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	auto chipSize = _mapCtrl.GetChipSize().ToVector2Int();
	DrawBox(offset + _mapPos * chipSize, offset + (_mapPos + 1) * chipSize, 0xffffff, true);

	// 説明表示
	auto wsize = Application::Instance().GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(500, 0, wsize.w, 100, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	int drawY = 0;
	DrawFormatString(500, drawY, 0xffffff, "MapEdit");
	drawY += 16;
	DrawFormatString(516, drawY, 0xffffff, "X : Z チップ変更    space チップ設置");
	drawY += 16;
	DrawFormatString(516, drawY, 0xffffff, "C : キャラクター配置へ変更");
}

void EditCursor::CharactorEditDraw()
{
	auto offset = _camera.GetCameraOffset();
	int alpha = abs(255 - (_animCnt % 512));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	if (_charactorChipInf.team == Team::player)
	{
		_mapCtrl.DrawSortieMass(offset, _charactorChipInf);
	}
	else
	{
		_mapCtrl.DrawCharactorChip(_charactorChipInf, offset);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);

	auto chipSize = _mapCtrl.GetChipSize();
	DrawBox(offset + _mapPos * chipSize, offset + (_mapPos + 1) * chipSize, 0xffffff, true);

	// 説明表示
	auto wsize = Application::Instance().GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(500, 0, wsize.w, 100, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	int drawY = 0;
	DrawFormatString(500, drawY, 0xffffff, "CharactorEdit");
	drawY += 16;
	DrawFormatString(516, drawY, 0xffffff, "X : Z チップ変更    space チップ設置");
	drawY += 16;
	DrawFormatString(516, drawY, 0xffffff, _charactorChipInf.team == Team::player ? "Team : Player" : "Team : Enemy");
	drawY += 16;
	DrawFormatString(516, drawY, 0xffffff, "Level. %d    + : P  - : O", _charactorChipInf.level);
	drawY += 16;
	DrawFormatString(516, drawY, 0xffffff, "C : マップチップ配置へ変更");
	drawY += 16;
	string moveActive("M : moveActive : ");
	moveActive = moveActive + (_charactorChipInf.active ? "true" : "false");
	DrawFormatString(516, drawY, 0xffffff, moveActive.c_str());
	drawY += 16;
	DrawFormatString(516, drawY, 0xffffff, "GroupNum %d    + : I  - : U", _charactorChipInf.groupNum + (_charactorChipInf.team == Team::enemy ?100 : 0));
}

void EditCursor::Draw()
{
	(this->*_uniqueDrawer)();
}
