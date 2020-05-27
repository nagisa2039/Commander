#pragma once
#include <vector>
#include <map>
#include "../Utility/Geometry.h"
#include <string>

class Animator
{
public:
	struct AnimInfo
	{
		std::vector<Rect> rectVec;
		int itv;
		bool isLoop;
	};

private:

	// �摜�̊i�[
	int _imageH;
	Size _imageSize;
	std::map<std::string, AnimInfo> _animInfoMap;

	// �A�j���[�V�������
	std::string _animName;
	int _animFrame;
	bool _animEnd;
	bool _reverseY;	// Y�����]

public:
	Animator();
	~Animator();

	void SetImage(const char* path);
	void SetImageHandle(const int handle);
	void SetReverseY(const bool value);

	void Update();
	void Draw(const Vector2Int& pos);
	void Draw(const Vector2Int& pos, Size size);

	void AddAnim(const std::string& animName, const AnimInfo& animInfo);
	void AddAnim(const std::string& animName, const std::vector<Rect>& rectvec, const int animItv, const bool loop);
	void AddAnimDiv(const std::string& animName, const Rect& startRect, const int animCnt, const int animItv, const bool loop, const bool vertical = true);

	void ChangeAnim(std::string animName);			// �A�j���[�V�����̕ύX
	void AnimRestart();	// �A�j���[�V�������͂��߂����蒼��

	const std::string& GetAnimName()const;
	const bool GetAnimEnd()const;
	const int GetAnimHandle()const;
	const Rect& GetAnimRect()const;
	const int GetImageH()const;
	const Size& GetImageSize()const;
};

