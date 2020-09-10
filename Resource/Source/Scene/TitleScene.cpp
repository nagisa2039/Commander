#include "TitleScene.h"
#include "FileSystem.h"
#include "SoundLoader.h"
#include <Dxlib.h>
#include "DxLibUtility.h"
#include "Fade.h"
#include "Input.h"
#include "Application.h"
#include "MapSelectScene.h"

void TitleScene::NormalUpdate(const Input& input)
{
	_animTrack->Update();
	if (input.GetButtonDown("ok"))
	{
		SoundL.PlaySE("Resource/Sound/SE/ok2.mp3");
		_fadeEndFunc = [&controller = _controller]() 
		{
			controller.ChangeScene(std::make_shared<MapSelectScene>(controller));
		};
		_controller.GetFade().StartFadeOut();
		_updater = &TitleScene::FadeUpdate;
	}
}

void TitleScene::FadeUpdate(const Input& input)
{
	if (_controller.GetFade().GetEnd())
	{
		_fadeEndFunc();
	}
}

TitleScene::TitleScene(SceneController& controller)
	:Scene(controller)
{
	_updater = &TitleScene::FadeUpdate;
	_fadeEndFunc = [&updater = _updater]() {updater = &TitleScene::NormalUpdate; };
	_controller.GetFade().StartFadeIn();
	_bgH = ImageHandle("Resource/Image/Title/title.png");
	_bgmH = SoundHandle("Resource/Sound/BGM/title.mp3");
	_start_bksH = ImageHandle("Resource/Image/Title/title_bks.png");
	SoundL.PlayBGM(_bgmH);

	_animTrack = std::make_shared<Track<float>>(true);
	_animTrack->AddKey(0, 0.0f);
	_animTrack->AddKey(30, 1.0f);
	_animTrack->AddKey(60, 0.0f);
}

TitleScene::~TitleScene()
{
	SoundL.StopSound(_bgmH);
}

void TitleScene::Update(const Input& input)
{
	(this->*_updater)(input);
}

void TitleScene::Draw(void)
{
	DrawGraph(0,0,_bgH, false);

	auto wsize = Application::Instance().GetWindowSize();
	Vector2Int center = Vector2Int(wsize.w / 2, wsize.h - 150);
	SetDrawBlendMode(DX_BLENDMODE_ADD, static_cast<int>(_animTrack->GetValue() * 255));
	SetDrawBright(10, 10, 255);
	DrawRotaGraph(center, 1.5, 0.0, _start_bksH, true);
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	int fontH = FontHandle("choplin60edge");
	DrawStringToHandle(center, Anker::center, 0xffffff, fontH, "ŠJŽn");
}
