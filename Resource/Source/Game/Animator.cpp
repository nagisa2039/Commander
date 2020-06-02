#include <DxLib.h>
#include "Animator.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"


Animator::Animator()
{
	_imageH = -1;
	_reverseY = false;
}


Animator::~Animator()
{
}

void Animator::SetImage(const char * path)
{
	_imageH = Application::Instance().GetFileSystem()->GetImageHandle(path);
	GetGraphSize(_imageH, &_imageSize.w, &_imageSize.h);
}

void Animator::SetImageHandle(const int handle)
{
	_imageH = handle;
}

void Animator::SetReverseY(const bool value)
{
	_reverseY = value;
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
	DrawRectRotaGraph(pos.x - rect.Width()/2, pos.y - rect.Height()/2, rect.Left(), rect.Top(), rect.Width(), rect.Height(), 1.0f, 0.0f, _imageH, true, _reverseY);
}

void Animator::Draw(const Vector2Int& pos, Size size)
{
	if (_animInfoMap.find(_animName) == _animInfoMap.end())
	{
		return;
	}

	auto rect = GetAnimRect();
	DrawRectRotaGraph(pos.x + size.w/2, pos.y + size.h/2, rect.Left(), rect.Top(), rect.Width(), rect.Height(), size.w / static_cast<float>(rect.Width()), 0.0f, _imageH, true, _reverseY);
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

void Animator::AddAnimDiv(const std::string& animName, const Vector2Int& animCnt, const int animItv, const bool loop)
{
	AnimInfo animInfo;
	animInfo.rectVec.reserve(animCnt.x * animCnt.y);

	auto graphSize = GetImageSize();
	auto rectSize = Size(graphSize.w / animCnt.x, graphSize.h / animCnt.y);
	auto rectCenter = rectSize.ToVector2Int() * 0.5;
	for (int i = 0; i < animCnt.x * animCnt.y; i++)
	{
		animInfo.rectVec.emplace_back(Rect(rectCenter, rectSize));
		if (i % animCnt.x == animCnt.x-1)
		{
			rectCenter.x = (rectSize.ToVector2Int() * 0.5f).x;
			rectCenter.y += rectSize.h;
		}
		else
		{
			rectCenter.x += rectSize.w;
		}
	}

	animInfo.itv = animItv;
	animInfo.isLoop = loop;
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
