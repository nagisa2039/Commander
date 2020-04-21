#include "TimeLine.h"

using namespace std;

TimeLine::TimeLine()
{
	_loop = false;
	_end = false;
}

TimeLine::~TimeLine()
{
}

void TimeLine::AddFloatTrack(const std::string& key, const uint32_t frame, const float value)
{
	if (_floatTrackMap.find(key) == _floatTrackMap.begin())
	{
		_floatTrackMap[key] = make_unique<Track<float>>();
	}
	_floatTrackMap[key]->AddKey(frame, value);
}

void TimeLine::AddVector2Track(const std::string& key, const uint32_t frame, const Vector2 value)
{
	if (_vector2TrackMap.find(key) == _vector2TrackMap.begin())
	{
		_vector2TrackMap[key] = make_unique<Track<Vector2>>();
	}
	_vector2TrackMap[key]->AddKey(frame, value);
}

void TimeLine::AddVector3Track(const std::string& key, const uint32_t frame, const Vector3 value)
{
	if (_vector3TrackMap.find(key) == _vector3TrackMap.begin())
	{
		_vector3TrackMap[key] = make_unique<Track<Vector3>>();
	}
	_vector3TrackMap[key]->AddKey(frame, value);
}

void TimeLine::Update()
{
	if (!_loop && _end)
	{
		return;
	}

	for (auto it = _floatTrackMap.begin(); it != _floatTrackMap.end(); it++)
	{
		it->second->Update();
	}
	for (auto it = _vector2TrackMap.begin(); it != _vector2TrackMap.end(); it++)
	{
		it->second->Update();
	}
	for (auto it = _vector3TrackMap.begin(); it != _vector3TrackMap.end(); it++)
	{
		it->second->Update();
	}

	// end ‚ÌŠm”F
	if (_end)
	{
		return;
	}

	for (auto it = _floatTrackMap.begin(); it != _floatTrackMap.end(); it++)
	{
		if (!it->second->GetLoop() && !it->second->GetEnd())
		{
			_end = false;
			return;
		}
	}
	for (auto it = _vector3TrackMap.begin(); it != _vector3TrackMap.end(); it++)
	{
		if (!it->second->GetLoop() && it->second->GetEnd())
		{
			_end = false;
			return;
		}
	}
	for (auto it = _vector2TrackMap.begin(); it != _vector2TrackMap.end(); it++)
	{
		if (!it->second->GetLoop() && it->second->GetEnd())
		{
			_end = false;
			return;
		}
	}
}

void TimeLine::Clear()
{
	_floatTrackMap.clear();
	_vector2TrackMap.clear();
	_vector3TrackMap.clear();
}

void TimeLine::Reset()
{
	for (auto it = _floatTrackMap.begin(); it != _floatTrackMap.end(); it++)
	{
		it->second->Reset();
	}
	for (auto it = _vector2TrackMap.begin(); it != _vector2TrackMap.end(); it++)
	{
		it->second->Reset();
	}
	for (auto it = _vector3TrackMap.begin(); it != _vector3TrackMap.end(); it++)
	{
		it->second->Reset();
	}
	_end = false;
}

bool TimeLine::GetEnd() const
{
	return _end;
}

float TimeLine::GetFloatVelue(const std::string& key) const
{
	return _floatTrackMap.at(key)->GetValue();
}

Vector2 TimeLine::GetVector2Velue(const std::string& key) const
{
	return _vector2TrackMap.at(key)->GetValue();
}

Vector3 TimeLine::GetVector3Velue(const std::string& key) const
{
	return _vector3TrackMap.at(key)->GetValue();
}
