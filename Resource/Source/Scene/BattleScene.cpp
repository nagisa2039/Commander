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
		// 死んでいたら終わる
		if (_leftBC.GetCharacotr().GetIsDying())
		{
			return false;
		}

		// 追撃判定
		if (_pursuit)
		{
			// 追撃済みなので戦闘を終了する
			_updater = &BattleScene::SceneEndAnim;
			return true;
		}
		else
		{
			PursuitAttack();
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
		if (_leftBC.GetCharacotr().GetStatus().heal) return false;

		// 死んでいたら終わる
		if (_rightBC.GetCharacotr().GetIsDying()) return false;

		// 追撃判定
		if (_pursuit)
		{
			_updater = &BattleScene::SceneEndAnim;
			return true;
		}

		// 攻撃範囲内か確認
		auto mapPosSub = _leftBC.GetCharacotr().GetMapPos() - _rightBC.GetCharacotr().GetMapPos();
		if (_rightBC.GetCharacotr().GetAttackRange().Hit(abs(mapPosSub.x) + abs(mapPosSub.y)) && !_rightBC.GetCharacotr().GetStartStatus().heal)
		{
			_rightBC.StartAttackAnim();
			_updater = &BattleScene::RightTurn;
		}
		else
		{
			PursuitAttack(false);
		}
	}
	return true;
}

bool BattleScene::PursuitAttack(const bool rightAttack)
{
	auto leftStatus = _leftBC.GetCharacotr().GetStatus();
	auto rightStatus = _rightBC.GetCharacotr().GetStatus();
	if (leftStatus.CheckPursuit(rightStatus))
	{
		_leftBC.StartAttackAnim();
		_updater = &BattleScene::LeftTurn;
		_pursuit = true;
		return true;
	}
	else
	{
		if (rightAttack && rightStatus.CheckPursuit(leftStatus))
		{
			_leftBC.StartAttackAnim();
			_updater = &BattleScene::RightTurn;
			_pursuit = true;
			return true;
		}
	}

	// 追撃不可なので終了
	_updater = &BattleScene::SceneEndAnim;
	return false;
}

BattleScene::BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl, const Vector2Int& cameraOffset)
	: Scene(ctrl), _screenSize(Application::Instance().GetWindowSize()), _leftBC(leftBC), _rightBC(rightBC), 
	_startPos(cameraOffset + (leftBC.GetCharacotr().GetCenterPos() + rightBC.GetCharacotr().GetCenterPos()).ToVector2Int()*0.5f)
{
	auto wsize = Application::Instance().GetWindowSize();
	auto cameraPos = wsize.ToVector2Int() * 0.5;
	_camera = make_shared<Camera>(Rect(cameraPos, wsize));
	_camera->SetPos(Vector3((cameraPos).ToVector2()));

	_screenH = MakeScreen(_screenSize.w, _screenSize.h, true);

	_effects.clear();


	_floatY = _screenSize.h / 2;
	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	auto distance = 0;
	auto mapPosSub = _leftBC.GetCharacotr().GetMapPos() - _rightBC.GetCharacotr().GetMapPos();
	distance = abs(mapPosSub.x) + abs(mapPosSub.y) <= 1 ? 200 : 300;

	_leftBC.Init(Vector2(screenCenter.x - distance, _floatY),  Dir::left,  &rightBC);
	_rightBC.Init(Vector2(screenCenter.x + distance, _floatY), Dir::right, &leftBC);

	_pursuit = false;

	_exRateTL = make_unique<Track<float>>();
	_exRateTL->ClearKey();
	_exRateTL->AddKey(0,0.0f);
	_exRateTL->AddKey(60, 1.0f);

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
	_leftBC.GetCharacotr().MoveEnd();
	_controller.PopScene();
}

void BattleScene::Draw(void)
{
	SetDrawScreen(_screenH);
	ClsDrawScreen();

	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	// 床の描画
	auto mapPosSub = _leftBC.GetCharacotr().GetMapPos() - _rightBC.GetCharacotr().GetMapPos();
	if (abs(mapPosSub.x) + abs(mapPosSub.y) <= 1)
	{
		int floorH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/Battle/floor_big.png");
		DrawRotaGraph(screenCenter.x, _floatY, 1.0, 0.0, floorH, true);
	}
	else
	{
		int floorH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/Battle/floor_small.png");
		DrawRotaGraph(_leftBC.GetStartPos(), 1.0, 0.0, floorH, true);
		DrawRotaGraph(_rightBC.GetStartPos(), 1.0, 0.0, floorH, true);
	}

	auto charSize = Size(128, 128);

	_leftBC.Draw();
	_rightBC.Draw();

	for (auto& effect : _effects)
	{
		effect->Draw();
	}

	if (!_exRateTL->GetEnd())
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - _exRateTL->GetValue() * 255.0f);
		DrawBox(0, 0, _screenSize.w, _screenSize.h, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	SetDrawScreen(DX_SCREEN_BACK);

	auto wsize = Application::Instance().GetWindowSize();

	if (_exRateTL->GetEnd())
	{
		DrawRotaGraph(Vector2Int(wsize.w / 2, wsize.h / 2), 1.0f, 0.0f, _screenH, true);
	}
	else
	{
		DrawRotaGraph(Lerp(_startPos, Vector2Int(wsize.w / 2, wsize.h / 2), _exRateTL->GetValue()), _exRateTL->GetValue(), _exRateTL->GetValue() * 4.0f * DX_PI, _screenH, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (1.0f - _brightTL->GetValue()) * 255);
	DrawBox(0,0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

std::vector<std::shared_ptr<Effect>>& BattleScene::GetEffectVec()
{
	return _effects;
}
