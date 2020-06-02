#include "MapEditScene.h"
#include <DxLib.h>
#include "../Game/MapCtrl.h"
#include "../Game/EditCursor.h"
#include "../Game/Camera.h"
#include "../System/Application.h"
#include "../Utility/Input.h"
#include "SceneController.h"
#include "PlayScene.h"

using namespace std;


MapEditScene::MapEditScene(SceneController& controller):Scene(controller)
{
    _camera = make_shared<Camera>(Rect(Vector2Int(), Application::Instance().GetWindowSize()));
    _mapCtrl = make_shared<MapCtrl>(_charactors);
    _editCursor = make_shared<EditCursor>(*_mapCtrl, *_camera);

   _camera->AddTargetActor(&(*_editCursor));

   auto mapSize = _mapCtrl->GetMapSize() * _mapCtrl->GetChipSize();
   _camera->SetLimitRect(Rect(mapSize.ToVector2Int()*0.5, mapSize));

   _mapCtrl->LoadMap();
}

MapEditScene::~MapEditScene()
{
}

void MapEditScene::Update(const Input& input)
{
    _editCursor->Update(input);

    _camera->Update();

    if (input.GetButtonDown(0, "F1") || input.GetButtonDown(1, "pause"))
    {
        _controller.ChangeScene(make_unique<PlayScene>(_controller));
        return;
    }
    if (input.GetButtonDown(0, "F2"))
    {
        _mapCtrl->SaveMap();
    }
    if (input.GetButtonDown(0, "F3"))
    {
        _mapCtrl->LoadMap();
    }
    if (input.GetButtonDown(0, "F5"))
    {

    }
    XINPUT_STATE xis;
    GetJoypadXInputState(DX_INPUT_PAD1, &xis);
    if (input.GetButtonDown(0, "pause") || input.GetXInputButtonDown(1, XINPUT_BUTTON_START))
    {
        _controller.ChangeScene(make_unique<PlayScene>(_controller));
        return;
    }
}

void MapEditScene::Draw(void)
{
    _mapCtrl->Draw(*_camera, true);

    _editCursor->Draw();

    DrawString(10, 10, "MAP EDIT SCENE", 0x00ffff);
    DrawString(10, 26, "F1 : PlayScene", 0x00ffff);
    DrawString(10, 42, "F2 : MapSave",   0x00ffff);
    DrawString(10, 58, "F3 : MapLoad",   0x00ffff);
}
