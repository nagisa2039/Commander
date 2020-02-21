#include "MapEditScene.h"
#include <DxLib.h>
#include "../Game/MapCtrl.h"
#include "../Game/EditCursor.h"
#include "../Game/Camera.h"
#include "../System/Application.h"

using namespace std;

MapEditScene::MapEditScene(SceneController& controller):Scene(controller)
{
    _camera = make_shared<Camera>(Rect(Vector2Int(), Application::Instance().GetWindowSize()));
    _mapCtrl = make_shared<MapCtrl>();
    _editCursor = make_shared<EditCursor>(*_mapCtrl);

   _camera->AddTargetActor(_editCursor);

    _mapCtrl->SaveMap("map0");
    _mapCtrl->LoadMap("map0");
}

MapEditScene::~MapEditScene()
{
}

void MapEditScene::Update(const Input& input)
{
    _editCursor->Update(input);

    _camera->Update();
}

void MapEditScene::Draw(void)
{
    _mapCtrl->Draw(*_camera, true);

    _editCursor->Draw(*_camera);

    DrawString(10,10, "MAP EDIT SCENE", 0x00ffff);
}
