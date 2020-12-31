#pragma once
#include <unordered_map>
#include <functional>

/// <summary>
/// ���\�[�X�̐�ǂ݂��s���N���X
/// </summary>
class AdvanceLoader
{
private:
	// �ǂݍ��ފ֐��}�b�v
	std::unordered_map<std::string, std::function<void(const std::string& path)>> _loadFuncMap;

public:
	AdvanceLoader();
	~AdvanceLoader();
};

