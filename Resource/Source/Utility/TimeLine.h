#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <map>
#include <string>
#include "Geometry.h"

/// <summary>
/// アニメーション用トラック
/// </summary>
/// <typeparam name="T">遷移させる型</typeparam>
template<typename T>
class Track
{
private:
	// 現在のフレーム数
	uint32_t _frame;
	using key = std::pair<uint32_t, T>;
	// キーデータ配列
	std::vector<key> _keys;
	// ループ
	bool _loop;
	// 終了
	bool _end;
	// 反転
	bool _reverse;

	// 更新関数ポインタ
	void (Track::* _updater)();

public:
	/// <param name="loop">ループ</param>
	inline Track(const bool loop = false)
	{
		_frame = 0;
		_keys.clear();
		_loop = loop;
		_end = false;
		_reverse = false;

		_updater = &Track::InitUpdate;
	}

	inline ~Track()
	{
	}

	/// <summary>
	/// 最初の更新時に呼ばれる
	/// </summary>
	inline void InitUpdate()
	{
		auto fuc = [](const key& l, const key& r)
		{
			return l.first < r.first;
		};
		std::sort(_keys.begin(), _keys.end(), fuc);
		NormalUpdate();
		_updater = &Track::NormalUpdate;
	}

	/// <summary>
	/// 通常更新
	/// </summary>
	inline void NormalUpdate()
	{
		_frame++;
		_end = _frame >= _keys.rbegin()->first;
	}

	/// <summary>
	/// 更新
	/// </summary>
	inline void Update()
	{
		(this->*_updater)();
	}

	/// <summary>
	/// リセット
	/// </summary>
	inline void Reset()
	{
		_frame = 0;
		_end = false;
	}

	/// <summary>
	/// キーの追加
	/// </summary>
	/// <param name="frame">フレーム数</param>
	/// <param name="value">値</param>
	inline void AddKey(const uint32_t frame, const T value)
	{
		_keys.emplace_back(std::make_pair(frame, value));

	}

	/// <summary>
	/// キーデータの削除
	/// </summary>
	inline void ClearKey()
	{
		_keys.clear();
	}

	/// <summary>
	/// 値の取得
	/// </summary>
	inline T GetValue()
	{
		if (_keys.size() <= 0)
		{
			return T();
		}

		unsigned int lastFrame = _keys.rbegin()->first;
		unsigned int calFrame = _loop ? _frame % lastFrame : _frame;
		if (calFrame < 0)
		{
			calFrame = 0;
		}
		if (calFrame > lastFrame)
		{
			calFrame = lastFrame;
		}
		calFrame = _reverse ? lastFrame - calFrame : calFrame;

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

		auto parsent = (calFrame - currnet->first) / static_cast<float>(next->first - currnet->first);
		return static_cast<T>(currnet->second + (next->second - currnet->second) * parsent);
	}

	/// <summary>
	/// 終了判定
	/// </summary>
	/// <returns></returns>
	inline bool GetEnd()const
	{
		return _end;
	}

	/// <summary>
	/// ループ
	/// </summary>
	/// <returns></returns>
	inline bool GetLoop()const
	{
		return _loop;
	}

	/// <summary>
	/// 反転設定
	/// </summary>
	/// <param name="value">反転</param>
	inline void SetReverse(const bool value)
	{
		_reverse = value;
	}

	/// <summary>
	/// 反転状態か
	/// </summary>
	inline bool GetReverse()const
	{
		return _reverse;
	}

	/// <summary>
	/// フレーム数の取得
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetFrame()const
	{
		return _frame;
	}

	/// <summary>
	/// 終了させる
	/// </summary>
	inline void End()
	{
		_frame = _keys.rbegin()->first;
		_end = true;
	}
};

using Track_f		= Track<float>;
using Track_i		= Track<int>;
using Track_vec2	= Track<Vector2>;
using Track_vec2Int = Track<Vector2Int>;
using Track_vec3	= Track<Vector3>;