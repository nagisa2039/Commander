#include "BattleScene.h"
#include "../Game/Charactor.h"
#include "DxLibUtility.h"
#include "../System/Application.h"
#include "../Utility/Input.h"
#include "../System/FileSystem.h"
#include "Effect/Effect.h"
#include "Camera.h"
#include <DxLib.h>
#include "Effect/FlyText.h"
#include "BattleCharactor.h"
#include "UI/Experience.h"
#include "SoundLoader.h"

using namespace std;

void BattleScene::SceneStartAnim(const Input& input)
{
	if (_exRateTL->GetEnd())
	{
		_leftBC.StartAttackAnim();
		_updater = &BattleScene::LeftTurn;
	}
}

void BattleScene::SceneEndAnim(const Input& input)
{
	_brightTL->Update();
	if (_brightTL->GetEnd())
	{
		End();
		return;
	}
}

void BattleScene::LeftTurn(const Input& input)
{
	_leftBC.AttackUpdate(*this);
	if (_leftBC.GetAttackAnimEnd())
	{
		_rightBC.StartHPAnim();
		_updater = &BattleScene::RightHPAnim;
	}
}

void BattleScene::LeftHPAnim(const Input& input)
{
	_leftBC.UIAnimUpdate();
	if (_leftBC.GetHPAnimEnd())
	{
		// 死んでいたら終わる
		if (_leftBC.GetCharacotr().GetIsDying())
		{
			StartExpUpdate();
			return;
		}

		// 追撃判定
		if (_pursuit)
		{
			// 追撃済みなので戦闘を終了する
			StartExpUpdate();
			return;
		}
		else
		{
			PursuitAttack();
		}
	}
}

void BattleScene::RightTurn(const Input& input)
{
	_rightBC.AttackUpdate(*this);
	if (_rightBC.GetAttackAnimEnd())
	{
		_leftBC.StartHPAnim();
		_updater = &BattleScene::LeftHPAnim;
	}
}

void BattleScene::RightHPAnim(const Input& input)
{
	_rightBC.UIAnimUpdate();
	if (_rightBC.GetHPAnimEnd())
	{
		if (_leftBC.GetCharacotr().GetBattleStatus().CheckHeal())
		{
			StartExpUpdate();
			return;
		}

		// 死んでいたら終わる
		if (_rightBC.GetCharacotr().GetIsDying())
		{
			StartExpUpdate();
			return;
		}

		// 追撃判定
		if (_pursuit)
		{
			StartExpUpdate();
			return;
		}

		// 攻撃範囲内か確認
		auto mapPosSub = _leftBC.GetCharacotr().GetMapPos() - _rightBC.GetCharacotr().GetMapPos();
		if (_rightBC.GetCharacotr().GetAttackRange().Hit(abs(mapPosSub.x) + abs(mapPosSub.y)) && !_rightBC.GetCharacotr().GetBattleStatus().CheckHeal())
		{
			_rightBC.StartAttackAnim();
			_updater = &BattleScene::RightTurn;
		}
		else
		{
			PursuitAttack(false);
		}
	}
}

void BattleScene::ExpUpdate(const Input& input)
{
	if (!_expUI)
	{
		// 追撃済みなので戦闘を終了する
		_updater = &BattleScene::SceneEndAnim;
		return;
	}

	_expUI->Update(input);
}

bool BattleScene::PursuitAttack(const bool rightAttack)
{
	auto leftBattleStatus = _leftBC.GetCharacotr().GetBattleStatus();
	auto rightBattleStatus = _rightBC.GetCharacotr().GetBattleStatus();
	if (leftBattleStatus.CheckPursuit(rightBattleStatus))
	{
		_leftBC.StartAttackAnim();
		_updater = &BattleScene::LeftTurn;
		_pursuit = true;
		return true;
	}
	else
	{
		if (rightAttack && rightBattleStatus.CheckPursuit(leftBattleStatus))
		{
			_leftBC.StartAttackAnim();
			_updater = &BattleScene::RightTurn;
			_pursuit = true;
			return true;
		}
	}

	// 追撃不可なので終了
	StartExpUpdate();
	return false;
}

void BattleScene::End()
{
	auto& soundLoader = Application::Instance().GetFileSystem().GetSoundLoader();
	soundLoader.StopSound();

	_leftBC.GetCharacotr().MoveEnd();
	_controller.PopScene();
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


	_floatY = _screenSize.h / 2.0f;
	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	auto distance = 0;
	auto mapPosSub = _leftBC.GetCharacotr().GetMapPos() - _rightBC.GetCharacotr().GetMapPos();
	distance = abs(mapPosSub.x) + abs(mapPosSub.y) <= 1 ? 200 : 300;

	_leftBC.Init(Vector2(static_cast<float>(screenCenter.x - distance), _floatY),  Dir::left,  &rightBC);
	_rightBC.Init(Vector2(static_cast<float>(screenCenter.x + distance), _floatY), Dir::right, &leftBC);

	_pursuit = false;

	_exRateTL = make_unique<Track<float>>();
	_exRateTL->ClearKey();
	_exRateTL->AddKey(0,0.0f);
	_exRateTL->AddKey(60, 1.0f);

	_brightTL = make_unique<Track<float>>();
	_brightTL->AddKey(0, 1.0f);
	_brightTL->AddKey(30, 0.0f);

	_updater = &BattleScene::SceneStartAnim;

	_expUI.reset();

	auto& soundLoader = Application::Instance().GetFileSystem().GetSoundLoader();
	soundLoader.PlayBGM("Resource/Sound/BGM/game_maoudamashii_3_theme11b.mp3", true);
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

	(this->*_updater)(input);

	if (_effects.size() > 0)
	{
		for (auto& effect : _effects)
		{
			effect->Update(input);
		}

		auto newEnd = remove_if(_effects.begin(), _effects.end(),
			[](const std::shared_ptr<Effect>& effect) { return effect->GetDelete(); });
		_effects.erase(newEnd, _effects.end());
	}
}

void BattleScene::StartExpUpdate()
{
	if (_leftBC.GetCharacotr().GetTeam() == Team::player)
	{
		_expUI = make_shared<Experience>(_leftBC, _rightBC.GetCharacotr().GetIsDying(), nullptr);
		_updater = &BattleScene::ExpUpdate;
		return;
	}
	if (_rightBC.GetCharacotr().GetTeam() == Team::player && _rightBC.GetGivenDamage() > 0)
	{
		_expUI = make_shared<Experience>(_rightBC, _leftBC.GetCharacotr().GetIsDying(), nullptr);
		_updater = &BattleScene::ExpUpdate;
		return;
	}
	_updater = &BattleScene::SceneEndAnim;
}

void BattleScene::Draw(void)
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_screenH);
	ClsDrawScreen();

	auto screenCenter = _screenSize.ToVector2Int() * 0.5f;

	// 床の描画
	auto mapPosSub = _leftBC.GetCharacotr().GetMapPos() - _rightBC.GetCharacotr().GetMapPos();
	if (abs(mapPosSub.x) + abs(mapPosSub.y) <= 1)
	{
		int floorH = Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/Battle/floor_big.png");
		DrawRotaGraph(screenCenter.x, static_cast<int>(_floatY), 1.0, 0.0, floorH, true);
	}
	else
	{
		int floorH = Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/Battle/floor_small.png");
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

	if (_expUI)
	{
		_expUI->Draw();
	}

	auto wsize = Application::Instance().GetWindowSize();
	auto exRateValue = _exRateTL->GetValue();

	SetDrawScreen(currentScreen);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>((exRateValue * 128)));
	DrawBox(Vector2Int(0, 0), wsize.ToVector2Int(), 0x000000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawRotaGraph(Lerp(_startPos, Vector2Int(wsize.w / 2, wsize.h / 2), exRateValue), exRateValue, exRateValue * 4.0 * DX_PI, _screenH, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>((1.0f - _brightTL->GetValue()) * 255));
	DrawBox(0,0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

std::vector<std::shared_ptr<Effect>>& BattleScene::GetEffectVec()
{
	return _effects;
}
