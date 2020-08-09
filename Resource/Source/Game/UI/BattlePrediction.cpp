#include "BattlePrediction.h"
#include "Application.h"
#include "FileSystem.h"
#include "Charactor.h"
#include "Input.h"
#include <Dxlib.h>
#include "DxLibUtility.h"
#include "DataBase.h"

int BattlePrediction::GetChengePoint(const Dir& dir, bool rightAttack, BattleStatus& selfStatus, BattleStatus& targetStatus)
{
	int chengePoint = 0;
	if (dir == Dir::left)
	{
		if (!selfStatus.CheckHeal() && rightAttack)
		{
			chengePoint = (targetStatus.CheckPursuit(selfStatus) ? 2 : 1) * targetStatus.GetDamage(selfStatus);
		}
		return chengePoint;
	}
	else
	{
		if (targetStatus.CheckHeal())
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

BattlePrediction::BattlePrediction(const Charactor& self, const Charactor& target, std::deque<std::shared_ptr<UI>>* uiDeque, const Vector2Int& attackStartPos, const Map_Chip mapChip)
	: _selfCharactor(self), _targetCharactor(target), _mapChip(mapChip), UI(uiDeque)
{
	auto sub = _targetCharactor.GetMapPos() - attackStartPos;
	_distance = abs(sub.x) + abs(sub.y);
	_hpAnimTrack = std::make_unique<Track<float>>(true);
	_hpAnimTrack->AddKey(0,0.0f);
	_hpAnimTrack->AddKey(60, 1.0f);
	_hpAnimTrack->AddKey(120, 0.0f);
}

BattlePrediction::~BattlePrediction()
{
}

void BattlePrediction::Update(const Input& input)
{
	_hpAnimTrack->Update();
}

void BattlePrediction::Draw()
{
	auto wsize = Application::Instance().GetWindowSize();
	Rect windowRect(wsize.ToVector2Int()*0.5f, Size(600, 600));

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	windowRect.Draw(0x000000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	Size iconSize(200, 200);
	int drawY = windowRect.Top() + iconSize.h/2 + 50;
	auto& fileSystem = Application::Instance().GetFileSystem();
	int fontH = fileSystem.GetFontHandle("choplin20edge");
	_selfCharactor.DrawCharactorIcon(	Rect(Vector2Int(windowRect.center.x - iconSize.w / 2, drawY), iconSize));
	_targetCharactor.DrawCharactorIcon(	Rect(Vector2Int(windowRect.center.x + iconSize.w / 2, drawY), iconSize));
	drawY += iconSize.h / 2 + 50;

	auto mapChipData		= Application::Instance().GetDataBase().GetMapChipData(_mapChip);
	auto selfBattleStatus	= _selfCharactor.GetBattleStatus();
	selfBattleStatus.defenseCorrection	 = mapChipData.defense;
	selfBattleStatus.avoidanceCorrection = mapChipData.avoidance;

	auto targetBattleStatus = _targetCharactor.GetBattleStatus();

	auto window1Handle = fileSystem.GetImageHandle("Resource/Image/UI/window1.png");

	// çUåÇÇ≥ÇÍÇΩë§Ç™îΩåÇÇ≈Ç´ÇÈÇ©
	bool rightAttack = _targetCharactor.GetAttackRange().Hit(_distance) && !targetBattleStatus.CheckHeal();

	// Ç†Ç∆Y300
	// HP 100
	DrawHPBer(drawY, windowRect, rightAttack, fontH);

	int distance = 250;
	auto DrawContent = [&](const char* name, const int leftValue, const int rightValue, 
		const unsigned int leftColor = 0xffffff, const unsigned int rightColor = 0xffffff)
	{
		char str[256];
		if (selfBattleStatus.CheckHeal())
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
		if (selfBattleStatus.CheckHeal())
		{
			DrawContent("âÒïú", selfBattleStatus.GetRecover(), targetBattleStatus.GetDamage(selfBattleStatus));
			return;
		}
		else
		{
			auto GetAttackColor = [](const float rate)
			{
				unsigned int color = 0xffffff;
				if (rate > 1.0f)
				{
					color = 0x00ff80;
				}
				else if (rate < 1.0f)
				{
					color = 0xff0000;
				}
				return color;
			};

			auto& dataBase = Application::Instance().GetDataBase();
			unsigned int selfColor		= GetAttackColor(Application::Instance().GetDataBase().
				GetAttributeRate(selfBattleStatus.weaponTypeData.attribute, targetBattleStatus.weaponTypeData.attribute));
			unsigned int targetColor	= GetAttackColor(Application::Instance().GetDataBase().
				GetAttributeRate(targetBattleStatus.weaponTypeData.attribute, selfBattleStatus.weaponTypeData.attribute));

			DrawContent("à–óÕ", selfBattleStatus.GetDamage(targetBattleStatus), 
				targetBattleStatus.GetDamage(selfBattleStatus), selfColor, targetColor);
		}
		auto& fileSystem = Application::Instance().GetFileSystem();
		int tag_playerHandle = fileSystem.GetImageHandle("Resource/Image/UI/tag_player.png");
		int tag_enemyHandle = fileSystem.GetImageHandle("Resource/Image/UI/tag_enemy.png");
		Size graphSize;
		GetGraphSize(tag_playerHandle, graphSize);
		auto leftCenter = Vector2Int(windowRect.center.x - (distance/2 - 50), drawY);
		auto rightCenter = Vector2Int(windowRect.center.x + (distance/2 - 50), drawY);
		if (selfBattleStatus.CheckPursuit(targetBattleStatus))
		{
			DrawGraph(GetDrawPos(leftCenter, graphSize, Anker::center), tag_playerHandle);
			DrawStringToHandle(leftCenter, Anker::center, 0xffffff, fontH, "Å~2");
		}
		if (rightAttack && targetBattleStatus.CheckPursuit(selfBattleStatus))
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
	DrawContent("ñΩíÜ", selfBattleStatus.GetHit(targetBattleStatus), targetBattleStatus.GetHit(selfBattleStatus));
	// ïKéE
	drawY += 40;
	DrawContent("ïKéE", selfBattleStatus.GetCritical(targetBattleStatus), targetBattleStatus.GetCritical(selfBattleStatus));
}

void BattlePrediction::DrawHPBer(int& drawY, const Rect& windowRect, bool rightAttack, int fontH)
{
	Size hpSize(200, 30);
	Size hpOutSize(250, 50);
	drawY += hpOutSize.h / 2;
	auto& FileSystem = Application::Instance().GetFileSystem();
	auto fukidashiH = FileSystem.GetImageHandle("Resource/Image/UI/fukidashi.png");
	Size fukisashiSize;
	GetGraphSize(fukidashiH, fukisashiSize);
	float fukidashiScale = 0.5f;
	fukisashiSize *= fukidashiScale;
	int choplin30 = FileSystem.GetFontHandle("choplin20");

	auto DrawHP = [&](const Dir dir, const Charactor& self, const Charactor& target)
	{
		Anker anker = dir == Dir::left ? Anker::rightcenter : Anker::leftcenter;
		auto teamColor = GetTeamColor(self.GetTeam());

		auto selfBattleStatus = self.GetBattleStatus();
		auto targetBattleStatus = target.GetBattleStatus();

		// HPë‰éÜ
		auto drawPos = GetDrawPos(Vector2Int(windowRect.center.x, drawY), hpOutSize, anker);
		DrawBox(drawPos, drawPos + hpOutSize, 0xffffff);
		// HPÇÃîwåi
		drawPos = GetDrawPos(Vector2Int(windowRect.center.x, drawY), hpSize, anker);
		DrawBox(drawPos, drawPos + hpSize, 0x888888);
		// HPÇÃïœìÆï™
		int chengePoint = GetChengePoint(dir, rightAttack, selfBattleStatus, targetBattleStatus);
		float before = static_cast<float>(self.GetStatus().health) / static_cast<float>(self.GetStartStatus().health);
		int affterHealth = min(max(self.GetStatus().health - chengePoint, 0), self.GetStartStatus().health);
		float affter = affterHealth / static_cast<float>(self.GetStartStatus().health);

		Size subSize(static_cast<int>(hpSize.w * (abs(before - affter)) + 2), hpSize.h);
		float sing = (dir == Dir::left ? -1 : 1);
		float sizeRate = (before + affter) / 2.0f * sing;
		drawPos = GetDrawPos(Vector2Int(windowRect.center.x + static_cast<int>(sizeRate * hpSize.w), drawY), subSize, Anker::center);

		auto hpAnimValue = _hpAnimTrack->GetValue();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(hpAnimValue * 128));
		DrawBox(drawPos, drawPos + subSize, teamColor);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		// écÇËHPÇÃêÅÇ´èoÇµï\é¶
		if (chengePoint != 0)
		{
			Vector2Int fukidashiDrawPos = drawPos + Vector2Int(subSize.w * (dir == Dir::left ? 1 : 0), -fukisashiSize.h / 2 - fukisashiSize.h/5 * hpAnimValue);
			DrawRotaGraph(fukidashiDrawPos, fukidashiScale, 0.0f, fukidashiH, true);
			DrawStringToHandle(fukidashiDrawPos, Anker::center, 0x000000, choplin30, "%d", affterHealth);
		}

		// HPÇÃécó 
		Size currentSize(static_cast<int>(hpSize.w * (targetBattleStatus.CheckHeal() ? before : affter)), hpSize.h);
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
	DrawRotaGraph(center, 1.1, 0.0f, Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/UI/window3.png"), true);
	DrawStringToHandle(center, Anker::center, 0xffffff, fontH, "HP");
	drawY += hpOutSize.h / 2;
}
