#pragma once
#include <unordered_map>
#include <functional>

/// <summary>
/// リソースの先読みを行うクラス
/// </summary>
class AdvanceLoader
{
private:
	// 読み込む関数マップ
	std::unordered_map<std::string, std::function<void(const std::string& path)>> _loadFuncMap;

public:
	AdvanceLoader();
	~AdvanceLoader();
};

