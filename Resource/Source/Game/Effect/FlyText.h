#pragma once
#include <memory>
#include "Effect.h"
#include "DxLibUtility.h"
#include "TimeLine.h"

class TextEffect :
	public Effect
{
protected:
	std::string _str;
	int _fontHandle;
	unsigned int _color;

public:
	TextEffect(const std::string& str, const Vector2Int& pos, 
		Camera& camera, const bool cameraActive);
	~TextEffect() = default;

	void Update(const Input& input) = 0;
	void Draw() = 0;

	void SetFontHandle(const int handle);
	void SetColor(const unsigned int color);
};

class FlyText :
	public TextEffect
{
private:
	Anker _anker;
	Vector2 _move;
	bool _critical;

	std::unique_ptr<Track<float>> _moveTrack;
	std::unique_ptr<Track<float>> _alphaTrack;

public:
	FlyText(const std::string& str, const Vector2Int& pos, const int lifeCnt, 
		Camera& camera, const bool cameraActive, const bool critical = false);
	~FlyText();

	void Update(const Input& input)override final;
	void Draw()override final;

	void SetAnker(const Anker anker);
	void SetMove(const Vector2& move);
};

class PopupText :
	public TextEffect
{
private:
	struct charInf
	{
		char c;
		Vector2Int startPos;
		float currentY;
		float velocityY;
		bool end;
	};

	std::vector<charInf> _charInfs;
	std::unique_ptr<Track_f> _moveTrack;
	std::unique_ptr<Track_f> _alphaTrack;
	int _currentIdx;

	Anker _anker;
	bool _critical;


public:
	PopupText(const std::string& str, const Vector2Int& pos, 
		Camera& camera, bool cameraActive, bool critical = false);
	~PopupText() = default;

	void Update(const Input& input)override;
	void Draw()override;
};