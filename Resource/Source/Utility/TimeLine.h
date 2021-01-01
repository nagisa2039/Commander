#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <map>
#include <string>
#include "Geometry.h"

/// <summary>
/// �A�j���[�V�����p�g���b�N
/// </summary>
/// <typeparam name="T">�J�ڂ�����^</typeparam>
template<typename T>
class Track
{
private:
	// ���݂̃t���[����
	uint32_t _frame;
	using key = std::pair<uint32_t, T>;
	// �L�[�f�[�^�z��
	std::vector<key> _keys;
	// ���[�v
	bool _loop;
	// �I��
	bool _end;
	// ���]
	bool _reverse;

	// �X�V�֐��|�C���^
	void (Track::* _updater)();

public:
	/// <param name="loop">���[�v</param>
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
	/// �ŏ��̍X�V���ɌĂ΂��
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
	/// �ʏ�X�V
	/// </summary>
	inline void NormalUpdate()
	{
		_frame++;
		_end = _frame >= _keys.rbegin()->first;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	inline void Update()
	{
		(this->*_updater)();
	}

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	inline void Reset()
	{
		_frame = 0;
		_end = false;
	}

	/// <summary>
	/// �L�[�̒ǉ�
	/// </summary>
	/// <param name="frame">�t���[����</param>
	/// <param name="value">�l</param>
	inline void AddKey(const uint32_t frame, const T value)
	{
		_keys.emplace_back(std::make_pair(frame, value));

	}

	/// <summary>
	/// �L�[�f�[�^�̍폜
	/// </summary>
	inline void ClearKey()
	{
		_keys.clear();
	}

	/// <summary>
	/// �l�̎擾
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

		// �w��t���[���̃A�j���[�V�����������̂łƂ΂�
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
	/// �I������
	/// </summary>
	/// <returns></returns>
	inline bool GetEnd()const
	{
		return _end;
	}

	/// <summary>
	/// ���[�v
	/// </summary>
	/// <returns></returns>
	inline bool GetLoop()const
	{
		return _loop;
	}

	/// <summary>
	/// ���]�ݒ�
	/// </summary>
	/// <param name="value">���]</param>
	inline void SetReverse(const bool value)
	{
		_reverse = value;
	}

	/// <summary>
	/// ���]��Ԃ�
	/// </summary>
	inline bool GetReverse()const
	{
		return _reverse;
	}

	/// <summary>
	/// �t���[�����̎擾
	/// </summary>
	/// <returns></returns>
	inline uint32_t GetFrame()const
	{
		return _frame;
	}

	/// <summary>
	/// �I��������
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