#pragma once
#include <vector>
#include <algorithm>

template<typename T>
class TimeLine
{
private:
	uint32_t _frame;
	using key = std::pair<uint32_t, T>;
	std::vector<key> _keys;
	bool _loop;
	bool _end;

public:

	inline TimeLine(const bool loop = false)
	{
		_keys.clear();
		_loop = loop;
		_end = false;
	}

	inline ~TimeLine()
	{
	}

	inline void Update()
	{
		_frame++;
	}

	inline void Reset()
	{
		_frame = 0;
	}

	inline void AddKey(const uint32_t frame, const T value)
	{
		_keys.emplace_back(std::make_pair(frame, value));

		auto fuc = [](const key& l, const key& r)
		{
			return l.first < r.first;
		};

		std::sort(_keys.begin(), _keys.end(), fuc);
	}

	inline void ClearKey()
	{
		_keys.clear();
	}

	inline T GetValue()
	{
		if (_keys.size() <= 0)
		{
			return T();
		}

		auto lastFrame = _keys.rbegin()->first;
		uint32_t calFrame = _loop && _frame > lastFrame ? _frame % lastFrame : _frame;

		if (!_loop)
		{
			_end = calFrame >= lastFrame;
		}


		auto it = find_if(_keys.rbegin(), _keys.rend(), [calFrame](const key key)
		{
			return calFrame >= key.first;
		});

		// 指定フレームのアニメーションが無いのでとばす
		if (it == _keys.rend())
		{
			return T();
		}

		auto currnet = it;
		auto next = it.base();

		if (_keys.end() == next)
		{
			return currnet->second;
		}

		auto parsent = (calFrame - currnet->first) / (float)(next->first - currnet->first);
		return currnet->second + (next->second - currnet->second) * parsent;
	}

	inline bool GetEnd()const
	{
		return _end;
	}
};

