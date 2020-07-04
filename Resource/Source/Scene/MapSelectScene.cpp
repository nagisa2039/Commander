#include "MapSelectScene.h"
#include "Application.h"
#include "DataBase.h"
#include "Camera.h"
#include "Input.h"
#include "DxLibUtility.h"
#include <Dxlib.h>
#include "FileSystem.h"
#include "PlayScene.h"
#include "SaveData.h"
#include "MapSelectCharactor.h"
#include "Fade.h"
#include "SoundLoader.h"
#include "UI/UIList/WeaponList.h"

using namespace std;

MapSelectScene::MapSelectScene(SceneController& controller):Scene(controller)
{
	auto screenSize = Application::Instance().GetWindowSize();
	auto screenCenter = screenSize.ToVector2Int() * 0.5f;

	_camera = make_unique<Camera>(Rect(screenCenter, screenSize));
	_moveStartTrack = make_unique<Track<int>>();
	_moveStartTrack->AddKey(0, 0);
	_moveStartTrack->AddKey(10, 0);

	_fade = make_unique<Fade>();
	_fade->StartFadeIn();
	_goPlayScene = false;

	_selectIdx = 0;

	_contentPosVec.clear();
	int idx = 0;
	int space = 500;
	for (const auto& mapData : Application::Instance().GetDataBase().GetMapDataTable())
	{
		_contentPosVec.emplace_back(screenCenter + Vector2Int(space * idx, 0));
		idx++;
	}
	assert(_contentPosVec.size() > 0);

	auto charactorDataVec = Application::Instance().GetSaveData().GetCharactorDataVec();
	assert(charactorDataVec.size() > 0);
	for (const auto& charactorData : charactorDataVec)
	{
		_mapSelectCharactors.emplace_back(make_unique<MapSelectCharactor>(*_camera, charactorData.charType));
		(*_mapSelectCharactors.rbegin())->SetPos(_contentPosVec.at(_selectIdx).ToVector2());
		(*_mapSelectCharactors.rbegin())->SetTargetPos(_contentPosVec.at(_selectIdx));
	}
	_camera->SetPos(Vector3(_contentPosVec.at(_selectIdx).ToVector2()));

	_camera->AddTargetActor(&**_mapSelectCharactors.begin());

	_dir = 1;
	_charactorIdx = 0;

	_debug = true;

	//------------------
	_weaponList = make_shared<WeaponList>(Rect(Vector2Int(400, 400), Size(400, 400)), nullptr);
	//------------------
}

MapSelectScene::~MapSelectScene()
{
}

void MapSelectScene::Update(const Input& input)
{
	_fade->Update();
	if (!_fade->GetEnd())return;

	if (_goPlayScene)
	{
		_controller.ChangeScene(make_shared<PlayScene>(_controller, _selectIdx));
		return;
	}

	if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		_fade->StartFadeOut();
		_goPlayScene = true;
		return;
	}

	MoveUpdate(input);

	for (auto& mapSelectCharactor : _mapSelectCharactors)
	{
		mapSelectCharactor->Update(input);
	}
	_camera->Update();
}

void MapSelectScene::MoveUpdate(const Input& input)
{
	CursorMove(input);

	MapSelectCharactorUpdate();
}

void MapSelectScene::MapSelectCharactorUpdate()
{
	if (_charactorIdx >= _mapSelectCharactors.size()) return;

	_moveStartTrack->Update();
	if (_moveStartTrack->GetEnd())
	{
		int charactorSpace = 50;
		_mapSelectCharactors[_charactorIdx]->SetTargetPos(
			_contentPosVec[_selectIdx] + Vector2Int(charactorSpace * _charactorIdx, 0) * static_cast<float>(_dir));
		_charactorIdx++;
		_moveStartTrack->Reset();
	}
}

void MapSelectScene::CursorMove(const Input& input)
{
	if ((*_mapSelectCharactors.begin())->GetIsMove())return;

	// 後続のキャラクターを今までの目的地に移動させる
	auto successorMove = [&]()
	{
		for (unsigned int idx = 1; idx < _mapSelectCharactors.size(); idx++)
		{
			_mapSelectCharactors[idx]->SetTargetPos(_contentPosVec.at(_selectIdx));
		}
	};

	if (input.GetButton(0, "right") || input.GetButton(1, "right"))
	{
		if (_selectIdx < _contentPosVec.size() - 1 && _selectIdx + 1 <= (_debug ? _contentPosVec.size() : Application::Instance().GetSaveData().GetMapNum()))
		{
			_dir = -1;
			successorMove();
			_selectIdx++;
			_charactorIdx = 0;
			_mapSelectCharactors[_charactorIdx]->SetTargetPos(_contentPosVec.at(_selectIdx));

			if (_mapSelectCharactors.size() > 1)
			{
				_charactorIdx++;
				_moveStartTrack->Reset();
			}
		}
	}
	if (input.GetButton(0, "left") || input.GetButton(1, "left"))
	{
		_dir = 1;
		if (_selectIdx > 0)
		{
			successorMove();
			_selectIdx--;
			_charactorIdx = 0;
			_mapSelectCharactors[_charactorIdx]->SetTargetPos(_contentPosVec.at(_selectIdx));
			if (_mapSelectCharactors.size() > 1)
			{
				_moveStartTrack->Reset();
				_charactorIdx++;
			}
		}
	}

	_weaponList->Update(input);
}

void MapSelectScene::Draw()
{
	auto offset = _camera->GetCameraOffset();
	auto wsize = Application::Instance().GetWindowSize();
	DrawBox(Vector2Int(0,0), wsize.ToVector2Int(), 0x364364);

	Size contentSize = Size(300, 200);
	auto mapDataVec = Application::Instance().GetDataBase().GetMapDataTable();
	int fontH = Application::Instance().GetFileSystem().GetFontHandle("choplin40edge");
	auto mapNum = _debug ? _contentPosVec.size() : Application::Instance().GetSaveData().GetMapNum();
	for (size_t idx = 0; idx < mapDataVec.size(); idx++)
	{
		auto contentRect = Rect(_contentPosVec[idx], contentSize);

		if (!contentRect.IsHit(_camera->GetRect())) continue;

		if (idx > mapNum)
		{
			SetDrawBright(64,64,64);
		}
		contentRect.Draw(offset, 0xffffff);
		contentRect.Draw(offset, 0x000000, false);

		DrawStringToHandle(offset + contentRect.center, Anker::center, 0xffffff, fontH, mapDataVec[idx].name.c_str());
		SetDrawBright(255, 255, 255);
	}

	for (auto rItr = _mapSelectCharactors.rbegin(); rItr != _mapSelectCharactors.rend(); rItr++)
	{
		(*rItr)->Draw();
	}

	_weaponList->Draw();

	_fade->Draw();
}