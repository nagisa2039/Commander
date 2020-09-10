#pragma once
#include <unordered_map>
#include <functional>

class AdvanceLoader
{
private:
	std::unordered_map<std::string, std::function<void(const std::string& path)>> _loadFuncMap;

public:
	AdvanceLoader();
	~AdvanceLoader();
};

