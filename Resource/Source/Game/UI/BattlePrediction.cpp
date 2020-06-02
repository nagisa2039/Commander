#include "BattlePrediction.h"
#include "Application.h"
#include "FileSystem.h"
#include "Charactor.h"
#include "Input.h"
#include <Dxlib.h>
#include "DxLibUtility.h"
#include "DataBase.h"

int BattlePrediction::GetChengePoint(const Dir& dir, bool rightAttack, Status& selfStatus, Status& targetStatus)
{
	int chengePoint = 0;
	if (dir == Dir::left)
	{
		if (!selfStatus.heal && rightAttack)
		{
			chengePoint = (targetStatus.CheckPursuit(selfStatus) ? 2 : 1) * targetStatus.GetDamage(selfStatus);
		}
		return chengePoint;
	}
	else
	{
		if (targetStatus.heal)
		{
			chengePoint = -targetStatus.GetRecover();
		}
		else
		{
			chengePoint = (targetStatus.CheckPursuit(selfStatus) ? 2 : 1) * targetStatus.GetDamage(selfStatus);
		}
		return chengePoint;
	}
	return chengePoint;
}

BattlePrediction::BattlePrediction(const Charactor& self, const Charactor& target, std::deque<std::shared_ptr<UI>> uiDeque, const unsigned int distance)
	: _selfCharactor(self), _targetCharactor(target), UI(uiDeque), _distance(distance)
{
	_hpAnimAlpha = std::make_unique<Track<float>>(true);
	_hpAnimAlpha->AddKey(0,0.0f);
	_hpAnimAlpha->AddKey(60, 1.0f);
	_hpAnimAlpha->AddKey(120, 0.0f);
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

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	windowRect.Draw(0x000000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	Size iconSize(200, 200);
	int drawY = windowRect.Top() + iconSize.h/2 + 50;
	int fontH = Application::Instance().GetFileSystem()->GetFontHandle("choplin20");
	_selfCharactor.DrawCharactorIcon(	Rect(Vector2Int(windowRect.center.x - iconSize.w / 2, drawY), iconSize));
	_targetCharactor.DrawCharactorIcon(	Rect(Vector2Int(windowRect.center.x + iconSize.w / 2, drawY), iconSize));
	drawY += iconSize.h / 2 + 50;


	auto selfStatus = _selfCharactor.GetStatus();
	auto targetStatus = _targetCharactor.GetStatus();

	auto window1Handle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/window1.png");

	// çUåÇÇ≥ÇÍÇΩë§Ç™îΩåÇÇ≈Ç´ÇÈÇ©
	bool rightAttack = _targetCharactor.GetAttackRange().Hit(_distance) && !targetStatus.heal;

	// Ç†Ç∆Y300
	// HP 100
	DrawHPBer(drawY, windowRect, rightAttack, fontH);

	int distance = 250;
	auto DrawContent = [&](const char* name, const int leftValue, const int rightValue, 
		const unsigned int leftColor = 0xffffff, const unsigned int rightColor = 0xffffff)
	{
		char str[256];
		if (selfStatus.heal)
		{
			sprintf_s(str, 256, "%d", leftValue);
			DrawStringToHandle(Vector2Int(windowRect.center.x - distance / 2, drawY), Anker::center, leftColor, fontH, str);
			DrawStringToHandle(Vector2Int(windowRect.center.x + distance / 2, drawY), Anker::center, 0xffffff, fontH, "-");
		}
		else
		{
			sprintf_s(str, 256, "%d", leftValue);
			DrawStringToHandle(Vector2Int(windowRect.center.x - distance / 2, drawY), Anker::center, leftColor, fontH, str);
			if (rightAttack)
			{
				sprintf_s(str, 256, "%d", rightValue);
				DrawStringToHandle(Vector2Int(windowRect.center.x + distance / 2, drawY), Anker::center, rightColor, fontH, str);
			}
			else
			{
				DrawStringToHandle(Vector2Int(windowRect.center.x + distance / 2, drawY), Anker::center, 0xffffff, fontH, "-");
			}
		}
		Rect nameRect(Vector2Int(windowRect.center.x, drawY), Size(100,30));
		DrawExtendGraph(nameRect.Left(), nameRect.Top(), nameRect.Right(), nameRect.Botton(), window1Handle, true);
		DrawStringToHandle(nameRect.center, Anker::center, 0xffffff, fontH, name);

	};

	auto DrawContentForPower = [&]()
	{
		if (selfStatus.heal)
		{
			DrawContent("âÒïú", selfStatus.GetRecover(), targetStatus.GetDamage(selfStatus));
			return;
		}
		else
		{
			auto GetAttackColor = [](const float rate)
			{
				float color = 0xffffff;
				if (rate > 1.01f)
				{
					color = 0x00ff80;
				}
				else if (rate < 0.99f)
				{
					color = 0xff0000;
				}
				return color;
			};

			unsigned int selfColor		= GetAttackColor(Application::Instance().GetDataBase().GetAttributeRate(selfStatus.attributeId, targetStatus.attributeId));
			unsigned int targetColor	= GetAttackColor(Application::Instance().GetDataBase().GetAttributeRate(targetStatus.attributeId, selfStatus.attributeId));

			DrawContent("à–óÕ", selfStatus.GetDamage(targetStatus), 
				targetStatus.GetDamage(selfStatus), selfColor, targetColor);
		}
		int tag_playerHandle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/tag_player.png");
		int tag_enemyHandle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/tag_enemy.png");
		Size graphSize;
		GetGraphSize(tag_playerHandle, graphSize);
		auto leftCenter = Vector2Int(windowRect.center.x - (distance/2 - 50), drawY);
		auto rightCenter = Vector2Int(windowRect.center.x + (distance/2 - 50), drawY);
		if (selfStatus.CheckPursuit(targetStatus))
		{
			DrawGraph(GetDrawPos(leftCenter, graphSize, Anker::center), tag_playerHandle);
			DrawStringToHandle(leftCenter, Anker::center, 0xffffff, fontH, "Å~2");
		}
		if (rightAttack && targetStatus.CheckPursuit(selfStatus))
		{
			DrawGraph(GetDrawPos(rightCenter, graphSize, Anker::center), tag_enemyHandle);
			DrawStringToHandle(rightCenter, Anker::center, 0xffffff, fontH, "Å~2");
		}
	};

	// 200
	// à–óÕ
	drawY += 40;
	DrawContentForPower();
	// ñΩíÜ
	drawY += 40;
	DrawContent("ñΩíÜ", selfStatus.GetHit(targetStatus), targetStatus.GetHit(selfStatus));
	// ïKéE
	drawY += 40;
	DrawContent("ïKéE", selfStatus.GetCritical(targetStatus), targetStatus.GetCritical(selfStatus));
}

void BattlePrediction::DrawHPBer(int& drawY, const Rect& windowRect, bool rightAttack, int fontH)
{
	Size hpSize(200, 30);
	Size hpOutSize(250, 50);
	drawY += hpOutSize.h / 2;

	auto DrawHP = [&](const Dir dir, const Charactor& self, const Charactor& target)
	{
		Anker anker = dir == Dir::left ? Anker::rightcenter : Anker::leftcenter;
		auto teamColor = GetTeamColor(self.GetTeam());

		auto selfStatus = self.GetStatus();
		auto targetStatus = target.GetStatus();

		// HPë‰éÜ
		auto drawPos = GetDrawPos(Vector2Int(windowRect.center.x, drawY), hpOutSize, anker);
		DrawBox(drawPos, drawPos + hpOutSize, 0xffffff);
		// HPÇÃîwåi
		drawPos = GetDrawPos(Vector2Int(windowRect.center.x, drawY), hpSize, anker);
		DrawBox(drawPos, drawPos + hpSize, 0x888888);
		// HPÇÃïœìÆï™
		int chengePoint = GetChengePoint(dir, rightAttack, selfStatus, targetStatus);
		float before = static_cast<float>(self.GetStatus().health) / static_cast<float>(self.GetStartStatus().health);
		int affterHealth = min(max(self.GetStatus().health - chengePoint, 0.0f), self.GetStartStatus().health);
		float affter = affterHealth / static_cast<float>(self.GetStartStatus().health);

		Size subSize(hpSize.w * (abs(before - affter)) + 2, hpSize.h);
		drawPos = GetDrawPos(Vector2Int(windowRect.center.x + (before + affter) / 2.0f * (dir == Dir::left ? -1 : 1) * hpSize.w, drawY), subSize, Anker::center);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _hpAnimAlpha->GetValue() * 128);
		DrawBox(drawPos, drawPos + subSize, teamColor);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// HPÇÃécó 
		Size currentSize(hpSize.w * (targetStatus.heal ? before : affter), hpSize.h);
		drawPos = GetDrawPos(Vector2Int(windowRect.center.x, drawY), currentSize, anker);
		DrawBox(drawPos, drawPos + currentSize, teamColor);

		char str[256];
		sprintf_s(str, 256, "%d", self.GetStatus().health);
		DrawStringToHandle(Vector2Int(windowRect.center.x + (dir == Dir::left ? -1 : 1) * (hpSize.w + 20), drawY), Anker::center, 0xffffff, fontH, str);
	};

	DrawHP(Dir::left, _selfCharactor, _targetCharactor);
	DrawHP(Dir::right, _targetCharactor, _selfCharactor);
	
	// çÄñ⁄ñºï\é¶
	auto center = Vector2Int(windowRect.center.x, drawY);
	DrawRotaGraph(center, 1.1, 0.0f, Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/window3.png"), true);
	DrawStringToHandle(center, Anker::center, 0xffffff, fontH, "HP");
	drawY += hpOutSize.h / 2;
}
