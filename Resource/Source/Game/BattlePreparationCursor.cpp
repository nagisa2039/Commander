#include "BattlePreparationCursor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
#include "Application.h"
#include "Charactor.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "UI/PreparationUI.h"
#include "UI/TerrainInf.h"
#include "UI/StatusInfomation.h"
#include "UI/StatusWindow.h"

using namespace std;

BattlePreparationCursor::BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera):MapCursor(mapCtrl, camera)
{
	_selectChar = nullptr;
	_end = false;

	_exRateTrack = std::make_unique<Track<float>>(true);
	_exRateTrack->AddKey(0, 0.8f);
	_exRateTrack->AddKey(30, 1.0f);
	_exRateTrack->AddKey(60, 0.8f);

	_terrainInfDeque.clear();
	_statusInfDeque.clear();
	_statusWindowDeque.clear();

	_terrainInf = make_shared<TerrainInf>(_terrainInfDeque, _mapCtrl, _mapPos);
	_terrainInfDeque.emplace_back(_terrainInf);

	_statusInf = make_shared<StatusInf>(_statusInfDeque, _mapCtrl, _mapPos);
	_statusInfDeque.emplace_back(_statusInf);
}

BattlePreparationCursor::~BattlePreparationCursor()
{
}

void BattlePreparationCursor::Update(const Input& input)
{
	bool open = _statusWindowDeque.size() > 0;
	if (!open)
	{
		CursorMove(input);
	}
	_exRateTrack->Update();

	if (open)
	{
		_terrainInf->Close();
		_statusInf->Close();
	}

	auto dequeUpdate = [&](std::deque<std::shared_ptr<UI>>& uiDeque)
	{
		if (uiDeque.size() <= 0)return;
		(*uiDeque.begin())->Update(input);
	};
	dequeUpdate(_statusWindowDeque);
	dequeUpdate(_terrainInfDeque);
	dequeUpdate(_statusInfDeque);

	if (!open)
	{
		if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
		{
			Select();
		}

		if (input.GetButtonDown(0, "back") || input.GetButtonDown(1, "back"))
		{
			_end = true;
		}

		if (input.GetButtonDown(0, "status") || input.GetButtonDown(1, "status"))
		{
			auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
			if (charactor == nullptr)return;
			_statusWindowDeque.emplace_back(make_shared<StatusWindow>(_statusWindowDeque, *charactor));
		}

		_terrainInf->Open();
		_statusInf->Open();
	}
}

void BattlePreparationCursor::Select()
{
	auto charactorChips = _mapCtrl.GetCharactorChipInf(_mapPos);
	auto charactor = _mapCtrl.GetMapPosChar(_mapPos);

	if(charactorChips.team != Team::player || charactorChips.type == CharactorType::max)return;

	// キャラクターの選択
	if (_selectChar == nullptr)
	{
		if (charactor == nullptr)return;

		_selectChar = charactor;
		return;
	}

	// 選択解除
	if (_selectChar == charactor)
	{
		_selectChar = nullptr;
		return;
	}

	// 空きマスなら
	if (charactor == nullptr)
	{
		if (charactorChips.team != Team::player)return;

		// 選択キャラをセット
		_selectChar->InitmapPos(charactorChips.mapPos);
	}
	else
	{
		// 選択キャラをそのマスにいるキャラと入れ替える
		charactor->InitmapPos(_selectChar->GetMapPos());
		_selectChar->InitmapPos(_mapPos);
	}

	// 移動したのでルートの再検索
	for (auto& charactor : _mapCtrl.GetCharacots())
	{
		charactor->RouteSearch();
	}
	_selectChar = nullptr;
}

void BattlePreparationCursor::Draw()
{
	auto chipSize = _mapCtrl.GetChipSize();
	auto offset = _camera.GetCameraOffset();
	
	auto handle = Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/MapChip/cursor.png");
	Size graphSize;
	GetGraphSize(handle, graphSize);

	DrawRectRotaGraph(offset + _mapPos * chipSize + chipSize * 0.5, Vector2Int(0, 0), graphSize,
		_exRateTrack->GetValue() * (chipSize.w / static_cast<float>(graphSize.w)), 0.0f, handle);

	auto dequeDraw = [&](std::deque<std::shared_ptr<UI>>& uiDeque)
	{
		for (auto rItr = uiDeque.rbegin(); rItr != uiDeque.rend(); rItr++)
		{
			(*rItr)->Draw();
		}
	};

	dequeDraw(_terrainInfDeque);
	dequeDraw(_statusInfDeque);
	dequeDraw(_statusWindowDeque);
}

void BattlePreparationCursor::DrawSortieMass()
{
	auto offset = _camera.GetCameraOffset();
	for (const auto& mapDataVec : _mapCtrl.GetMapData())
	{
		for (const auto& mapData : mapDataVec)
		{
			if (mapData.charactorChip.team != Team::player)continue;

			if (_selectChar != nullptr && mapData.charactorChip.mapPos == _selectChar->GetMapPos())
			{
				_mapCtrl.DrawSortieMass(offset, mapData.charactorChip, 0xffff00, 0xffff00);
			}
			else
			{
				_mapCtrl.DrawSortieMass(offset, mapData.charactorChip);
			}
		}
	}
}

void BattlePreparationCursor::DrawMovableMass()
{
	auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
	if (charactor == nullptr) return;

	charactor->DrawMovableMass(128);
}

bool BattlePreparationCursor::GetEnd() const
{
	return _end;
}

void BattlePreparationCursor::Start()
{
	_end = false;
	_camera.AddTargetActor(this);
	SetCursorStartPosition();
}

void BattlePreparationCursor::SetCursorStartPosition()
{
	for (const auto& charactor : _mapCtrl.GetCharacots())
	{
		if (charactor->GetTeam() == Team::player)
		{
			_mapPos = charactor->GetMapPos();
			_camera.SetPos(charactor->GetCenterPos());
			break;
		}
	}
}
