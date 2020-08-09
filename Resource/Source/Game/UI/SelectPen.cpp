#include <Dxlib.h>
#include "SelectPen.h"
#include "Application.h"
#include "FileSystem.h"
#include "DxLibUtility.h"

SelectPen::SelectPen(std::deque<std::shared_ptr<UI>>* uiDeque):UI(uiDeque)
{
	_penAnimTrack = std::make_unique<Track<float>>(true);
	_penAnimTrack->AddKey(0, 0.0f);
	_penAnimTrack->AddKey(30, 1.0f);
	_penAnimTrack->AddKey(60, 0.0f);
}

SelectPen::~SelectPen()
{
}

void SelectPen::Update(const Input& input)
{
	_penAnimTrack->Update();
}

void SelectPen::Draw()
{
}

void SelectPen::Draw(const Vector2Int& pos)
{
	auto& fileSystem = Application::Instance().GetFileSystem();
	auto penH = fileSystem.GetImageHandle("Resource/Image/UI/quillPen.png");
	Size penSize;
	GetGraphSize(penH, penSize);
	Vector2Int penMove = penSize.ToVector2Int() * -0.2f;

	Vector2Int penDrawPos = penMove * _penAnimTrack->GetValue() + pos + Vector2Int(10,-10);
	DrawGraph(GetDrawPos(penDrawPos, penSize, Anker::rightcenter), penH, true);
}