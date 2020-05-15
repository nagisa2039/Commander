#include "BattlePrediction.h"
#include "Application.h"
#include "FileSystem.h"
#include "Charactor.h"
#include "Input.h"
#include <Dxlib.h>
#include "DxLibUtility.h"

BattlePrediction::BattlePrediction(const Charactor& self, const Charactor& target, std::deque<std::shared_ptr<UI>> uiDeque, const Astar::ResultPos& attackPos)
	: _selfCharactor(self), _targetCharactor(target), UI(uiDeque), _attackPos(attackPos)
{
	_hpAnimAlpha = std::make_unique<Track<float>>(true);
	_hpAnimAlpha->AddKey(0,0.0f);
	_hpAnimAlpha->AddKey(60, 1.0f);
}

BattlePrediction::~BattlePrediction()
{
}

void BattlePrediction::Update(const Input& input)
{
	_hpAnimAlpha->Update();
}

void BattlePrediction::Draw()
{
	auto wsize = Application::Instance().GetWindowSize();
	Rect windowRect(wsize.ToVector2Int()*0.5f, Size(400, 600));
	windowRect.Draw(0x000000);


	Size iconSize(200, 200);
	int drawY = windowRect.Top() + iconSize.h/2 + 50;
	int fontH = Application::Instance().GetFileSystem()->GetFontHandle("choplin20");
	_selfCharactor.DrawCharactorIcon(	Rect(Vector2Int(windowRect.center.x + iconSize.w / 2, drawY), iconSize));
	_targetCharactor.DrawCharactorIcon(	Rect(Vector2Int(windowRect.center.x + iconSize.w / 2, drawY), iconSize));
	drawY += iconSize.h / 2 + 50;

	// Ç†Ç∆Y300
	// HP 100
	Size hpOutSize(250, 100);
	Size hpSize(200, 50);
	drawY += hpOutSize.h / 2;

	auto DrawHP = [&](const Dir dir, const Charactor& self, const Charactor& target)
	{
		Anker anker = dir == Dir::left ? Anker::rightcenter : Anker::leftcenter;

		// HPë‰éÜ
		auto drawPos = GetDrawPos(Vector2Int(windowRect.center.x, drawY), hpOutSize, anker);
		DrawBox(drawPos, drawPos + hpSize, 0xffffff);
		// HPÇÃîwåi
		drawPos = GetDrawPos(Vector2Int(windowRect.center.x, drawY), hpSize, anker);
		DrawBox(drawPos, drawPos + hpSize, 0xffffff);
		// HPÇÃå∏è≠ï™
		auto selfStatus = self.GetStatus();
		auto targetStatus = target.GetStatus();
		auto damage = (selfStatus.CheckPursuit(targetStatus) ? 2 : 1) * selfStatus.GetDamage(targetStatus);
		float before = static_cast<float>(self.GetStatus().health) / static_cast<float>(self.GetStartStatus().health);
		float affter = static_cast<float>(max(self.GetStatus().health - damage, 0.0f))  / static_cast<float>(self.GetStartStatus().health);

		Size subSize(hpSize.w * (before - affter), hpSize.h);
		drawPos = GetDrawPos(Vector2Int(windowRect.center.x + (before + affter)/2 * (dir == Dir::left ? - 1 : 1), drawY), subSize, Anker::center);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _hpAnimAlpha->GetValue() * 255);
		DrawBox(drawPos, drawPos + subSize, 0xffffff);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// HPÇÃécó 
		Size currentSize(hpSize.w * affter, hpSize.h);
		drawPos = GetDrawPos(Vector2Int(windowRect.center.x, drawY), currentSize, anker);
		DrawBox(drawPos, drawPos + currentSize, 0xffffff);
	};

	DrawHP(Dir::left, _selfCharactor, _targetCharactor);
	DrawHP(Dir::right, _targetCharactor, _selfCharactor);

	drawY += hpOutSize.h / 2;
	// 200
	// à–óÕ
	drawY += 40;
	auto DrawContent = [&](const char* name, const int leftValue, const int rightValue)
	{
		int distance = 200;
		char str[256];
		sprintf_s(str, 256, "%d", leftValue);
		DrawStringToHandle(Vector2Int(windowRect.center.x - distance/2, drawY), Anker::center, 0xffffff, fontH, str);
		sprintf_s(str, 256, "%d", rightValue);
		DrawStringToHandle(Vector2Int(windowRect.center.x + distance/2, drawY), Anker::center, 0xffffff, fontH, str);
		Rect nameRect(Vector2Int(windowRect.center.x, drawY), Size(100,30));
		nameRect.Draw(0x0000aa);
		DrawStringToHandle(nameRect.center, Anker::center, 0xffffff, fontH, str);

	};

	auto selfStatus = _selfCharactor.GetStatus();
	auto targetStatus = _targetCharactor.GetStatus();
	DrawContent("à–óÕ", selfStatus.GetDamage(_targetCharactor.GetStatus()), targetStatus.GetDamage(_selfCharactor.GetStatus()));
	DrawContent("ñΩíÜ", selfStatus.GetHit(targetStatus), targetStatus.GetHit(selfStatus));
	DrawContent("ïKéE", selfStatus.GetCritical(targetStatus), targetStatus.GetCritical(selfStatus));

	// ñΩíÜ
	drawY += 40;
	// ïKéE
	drawY += 40;
}
