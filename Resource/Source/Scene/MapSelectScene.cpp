#include "MapSelectScene.h"
#include "Application.h"
#include "DataBase.h"
#include "Camera.h"
#include "Input.h"
#include "DxLibUtility.h"
#include <Dxlib.h>
#include "FileSystem.h"
#include "PlayScene.h"

using namespace std;

MapSelectScene::MapSelectScene(SceneController& controller):Scene(controller)
{
	auto screenSize = Application::Instance().GetWindowSize();
	auto screenCenter = screenSize.ToVector2Int() * 0.5f;

	_camera = make_unique<Camera>(Rect(screenCenter, screenSize));

	_contentPosVec.clear();
	int idx = 0;
	int space = 500;
	for (const auto& mapData : Application::Instance().GetDataBase().GetMapDataTable())
	{
		_contentPosVec.emplace_back(screenCenter + Vector2Int(space * idx, 0));
		idx++;
	}
	assert(_contentPosVec.size() > 0);
	_camera->SetPos(Vector3((*_contentPosVec.begin()).ToVector2()));

	_selectIdx = 0;
}

MapSelectScene::~MapSelectScene()
{
}

void MapSelectScene::Update(const Input& input)
{
	if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		_controller.ChangeScene(make_shared<PlayScene>(_controller, _selectIdx));
		return;
	}

	if (input.GetButtonDown(0, "right") || input.GetButtonDown(1, "right"))
	{
		if (_selectIdx < _contentPosVec.size() - 1)
		{
			_selectIdx++;
			_camera->SetPos(Vector3(_contentPosVec.at(_selectIdx).ToVector2()));
		}
	}
	if (input.GetButtonDown(0, "left") || input.GetButtonDown(1, "left"))
	{
		if (_selectIdx > 0)
		{
			_selectIdx--;
			_camera->SetPos(Vector3(_contentPosVec.at(_selectIdx).ToVector2()));
		}
	}
	_camera->Update();
}

void MapSelectScene::Draw()
{
	auto offset = _camera->GetCameraOffset();
	auto wsize = Application::Instance().GetWindowSize();
	DrawBox(Vector2Int(0,0), wsize.ToVector2Int(), 0x114514);

	Size contentSize = Size(300, 200);
	auto mapDataVec = Application::Instance().GetDataBase().GetMapDataTable();
	int fontH = Application::Instance().GetFileSystem()->GetFontHandle("choplin40");
	for (size_t idx = 0; idx < mapDataVec.size(); idx++)
	{
		auto contentRect = Rect(_contentPosVec[idx], contentSize);

		if (!contentRect.IsHit(_camera->GetRect())) continue;

		contentRect.Draw(offset, 0xffffff);
		contentRect.Draw(offset, 0x000000, false);


		DrawStringToHandle(offset + contentRect.center, Anker::center, 0xffffff, fontH, mapDataVec[idx].name.c_str());
	}
}