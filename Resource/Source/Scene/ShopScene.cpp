#include <Dxlib.h>
#include "ShopScene.h"
#include "UI/UIList/CharactorList.h"
#include "Application.h"
#include "SaveData.h"
#include "Input.h"

using namespace std;

ShopScene::ShopScene(SceneController& controller):Scene(controller)
{
	/*_charactorList = make_shared<CharactorList>(Vector2Int(200, 100), 
		Application::Instance().GetSaveData().GetCharactorDataVec(), &_uiDeque);
	_uiDeque.push_front(_charactorList);*/
}

ShopScene::~ShopScene()
{
}

void ShopScene::Update(const Input& input)
{
	if (_uiDeque.size() > 0)
	{
		(*_uiDeque.begin())->Update(input);
	}
	else
	{
		_controller.PopScene();
	}
}

void ShopScene::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
	auto wsize = Application::Instance().GetWindowSize();
	DrawBox(0,0,wsize.w,wsize.h,0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//for(auto rItr = _uiDeque.rbegin(); rItr != _uiDeque.rend(); rItr++)
	//{
	//	(*rItr)->Draw();
	//}

	if (_uiDeque.size() > 0)
	{
		(*_uiDeque.begin())->Draw();
	}
}
