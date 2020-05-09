#include "BattleScene.h"
#include "../Game/Charactor.h"
#include "DxLibUtility.h"
#include "../System/Application.h"
#include "../Utility/Input.h"
#include "../System/FileSystem.h"
#include "Effect.h"
#include "Camera.h"
#include <DxLib.h>
#include "FlyText.h"
#include "BattleCharactor.h"

using namespace std;

bool BattleScene::SceneStartAnim(const Input& input)
{
	if (_exRateTL->GetEnd())
	{
		_leftBC.StartAttackAnim();
		_updater = &BattleScene::LeftTurn;
	}
	return true;
}

bool BattleScene::SceneEndAnim(const Input& input)
{
	_brightTL->Update();
	return !_brightTL->GetEnd();
}

bool BattleScene::LeftTurn(const Input& input)
{
	_leftBC.AttackUpdate(*this);
	if (_leftBC.GetAttackAnimEnd())
	{
		_rightBC.StartHPAnim();
		_updater = &BattleScene::RightHPAnim;
	}
	return true;
}

bool BattleScene::LeftHPAnim(const Input& input)
{
	_leftBC.UIAnimUpdate();
	if (_leftBC.GetHPAnimEnd())
	{
		// Ž€‚ñ‚Å‚¢‚½‚çI‚í‚é
		if (_leftBC.GetSelfCharacotr().GetIsDying())
		{
			return false;
		}

		// ’ÇŒ‚”»’è
		if (_pursuit)
		{
			_updater = &BattleScene::SceneEndAnim;
		}
		else
		{
			PursuitAttack();
			_pursuit = true;
		}
	}
	return true;
}

bool BattleScene::RightTurn(const Input& input)
{
	_rightBC.AttackUpdate(*this);
	if (_rightBC.GetAttackAnimEnd())
	{
		_leftBC.StartHPAnim();
		_updater = &BattleScene::LeftHPAnim;
	}
	return true;
}

bool BattleScene::RightHPAnim(const Input& input)
{
	_rightBC.UIAnimUpdate();
	if (_rightBC.GetHPAnimEnd())
	{
		// Ž€‚ñ‚Å‚¢‚½‚çI‚í‚é
		if (_rightBC.GetSelfCharacotr().GetIsDying())
		{
			return false;
		}

		// ’ÇŒ‚”»’è
		if (_pursuit)
		{
			_updater = &BattleScene::SceneEndAnim;
		}
		_rightBC.StartAttackAnim();
		_updater = &BattleScene::RightTurn;
	}
	return true;
}

bool BattleScene::PursuitAttack()
{
	auto leftStatus = _leftBC.GetSelfCharacotr().GetStatus();
	auto rightStatus = _rightBC.GetSelfCharacotr().GetStatus();
	if (leftStatus.speed - rightStatus.speed >= 4)
	{
		_leftBC.StartAttackAnim();
		_updater = &BattleScene::LeftTurn;
		return true;
	}
	else
	{
		if (rightStatus.speed - leftStatus.speed >= 4)
		{
			_leftBC.StartAttackAnim();
			_updater = &BattleScene::RightTurn;
			return true;
		}
	}
	return false;
}

BattleScene::BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl)
	: Scene(ctrl), _screenSize(Application::Instance().GetWindowSize()), _leftBC(leftBC), _rightBC(rightBC)
{
	auto wsize = Application::Instance().GetWindowSize();
	auto cameraPos = wsize.ToVector2Int() * 0.5;
	_camera = make_shared<Camera>(Rect(cameraPos, wsize));
	_camera->SetPos(Vector3((cameraPos).ToVector2()));

	_screenH = MakeScreen(_screenSize.w, _screenSize.h, true);

	_effects.clear();


	_floatY = _screenSize.h / 2;
	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	_leftBC.Init(Vector2(screenCenter.x - 200, _floatY),  Dir::left,  &rightBC);
	_rightBC.Init(Vector2(screenCenter.x + 200, _floatY), Dir::right, &leftBC);

	_pursuit = false;

	_exRateTL = make_unique<Track<float>>();
	_exRateTL->ClearKey();
	_exRateTL->AddKey(0,0.0f);
	_exRateTL->AddKey(30, 1.0f);

	_brightTL = make_unique<Track<float>>();
	_brightTL->AddKey(0, 1.0f);
	_brightTL->AddKey(30, 0.0f);

	_updater = &BattleScene::SceneStartAnim;
}

BattleScene::~BattleScene()
{
}

void BattleScene::Update(const Input& input)
{
	_exRateTL->Update();

	_camera->Update();

	_leftBC.AnimUpdate();
	_rightBC.AnimUpdate();

	if (!(this->*_updater)(input))
	{
		BattleEnd();
		return;
	}

	for (auto& effect : _effects)
	{
		effect->Update(input);
	}
	auto newEnd = remove_if(_effects.begin(), _effects.end(),
		[](const std::shared_ptr<Effect>& effect) { return effect->GetDelete(); });
	_effects.erase(newEnd, _effects.end());
}

void BattleScene::BattleEnd()
{
	_leftBC.GetSelfCharacotr().MoveEnd();
	_controller.PopScene();
}

void BattleScene::Draw(void)
{
	SetDrawScreen(_screenH);
	ClsDrawScreen();

	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	// °‚Ì•`‰æ
	DrawRotaGraph(screenCenter.x, _floatY, 1.0, 0.0,
		Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/Battle/floor.png"), true);

	auto charSize = Size(128, 128);

	_leftBC.Draw();
	_rightBC.Draw();

	for (auto& effect : _effects)
	{
		effect->Draw();
	}

	SetDrawScreen(DX_SCREEN_BACK);

	auto wsize = Application::Instance().GetWindowSize();

	DrawRotaGraph(wsize.w/2, wsize.h/2, _exRateTL->GetValue(), _exRateTL->GetValue() * 4.0f * DX_PI, _screenH, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (1.0f - _brightTL->GetValue()) * 255);
	DrawBox(0,0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	ScreenFlip();
}

std::vector<std::shared_ptr<Effect>>& BattleScene::GetEffectVec()
{
	return _effects;
}
