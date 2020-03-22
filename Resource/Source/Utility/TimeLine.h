#pragma once
#include <vector>
#include <algorithm>

template<typename T>
class TimeLine
{
private:

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

	inline void AddKey(const key& k)
	{
		_keys.emplace_back(k);

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

	inline T GetValue(const uint32_t frame)
	{
		if (_keys.size() <= 0)
		{
			return T();
		}

		auto lastFrame = _keys.rbegin()->first;
		uint32_t calFrame = _loop ? frame % lastFrame : frame;

		if (!_loop)
		{
			_end = frame >= lastFrame;
		}

		auto it = find_if(_keys.rbegin(), _keys.rend(), [frame](const key key)
		{
			return frame >= key.first;
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

		auto parsent = (frame - currnet->first) / (float)(next->first - currnet->first);
		return currnet->second + (next->second - currnet->second) * parsent;
	}

	inline bool GetEnd()const
	{
		return _end;
	}
};

