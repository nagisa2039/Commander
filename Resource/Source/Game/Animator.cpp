#include <DxLib.h>
#include "Animator.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"


Animator::Animator()
{
	_imageH = -1;
}


Animator::~Animator()
{
}

void Animator::SetImage(const char * path)
{
	_imageH = Application::Instance().GetFileSystem()->GetImageHandle(path);
	GetGraphSize(_imageH, &_imageSize.w, &_imageSize.h);
}

void Animator::Update()
{
	if (_animEnd)
	{
		return;
	}

	auto animInfo = _animInfoMap[_animName];
	if (++_animFrame%animInfo.itv == 0)
	{
		if (_animFrame/ animInfo.itv >= animInfo.rectVec.size())
		{
			if (animInfo.isLoop)
			{
				_animFrame = 0;
			}
			else
			{
				_animEnd = true;
				_animFrame--;
			}
		}
	}
}

void Animator::Draw(const Vector2Int & pos)
{
	if (_animInfoMap.find(_animName) == _animInfoMap.end())
	{
		return;
	}

	auto rect = GetAnimRect();
	DrawRectGraph(pos.x, pos.y, rect.Left(), rect.Top(), rect.Width(), rect.Height(), _imageH, true);
}

void Animator::Draw(const Vector2Int& pos, Size size)
{
	if (_animInfoMap.find(_animName) == _animInfoMap.end())
	{
		return;
	}

	auto rect = GetAnimRect();
	DrawRectExtendGraph(pos.x, pos.y, pos.x + size.h, pos.y + size.w, rect.Left(), rect.Top(), rect.Width(), rect.Height(), _imageH, true);
}

void Animator::AddAnim(const std::string& animName, const AnimInfo& animInfo)
{
	_animInfoMap.emplace(animName, animInfo);
}

void Animator::AddAnim(const std::string & animName, const std::vector<Rect>& rectVec, const int animItv, const bool loop)
{
	AnimInfo animInfo = {};
	animInfo.rectVec = rectVec;
	animInfo.itv = animItv;
	animInfo.isLoop = loop;
	_animInfoMap.emplace(animName, animInfo);
}

void Animator::AddAnimDiv(const std::string & animName, const Rect & startRect, const int animCnt, const int animItv, const bool loop, const bool vertical)
{
	AnimInfo animInfo;
	animInfo.rectVec.resize(animCnt);
	for (int j = 0; j < animCnt; j++)
	{
		auto rect = startRect;
		if (vertical)
		{
			rect.center.y += startRect.Height() * j;
		}
		else
		{
			rect.center.x += startRect.Width() * j;
		}
		animInfo.rectVec[j] = rect;
		animInfo.itv = animItv;
		animInfo.isLoop = loop;
	}
	_animInfoMap.emplace(animName, animInfo);
}

void Animator::ChangeAnim(std::string animName)
{
	if (_animName == animName)
	{
		return;
	}
	_animName = animName;
	AnimRestart();
}

void Animator::AnimRestart()
{
	_animEnd = false;
	_animFrame = 0;
}

const std::string & Animator::GetAnimName() const
{
	return _animName;
}

const bool Animator::GetAnimEnd() const
{
	return _animEnd;
}

const int Animator::GetAnimHandle() const
{
	return _imageH;
}

const Rect & Animator::GetAnimRect() const
{
	return _animInfoMap.at(_animName).rectVec[_animFrame/ _animInfoMap.at(_animName).itv];
}

const int Animator::GetImageH() const
{
	return _imageH;
}

const Size & Animator::GetImageSize() const
{
	return _imageSize;
}
