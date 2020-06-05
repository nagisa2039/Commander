#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include <array>
#include <functional>

class BattlePreparationCursor;
class MapCtrl;
class Camera;

class PreparationUI :
	public UI
{
private:
	enum class Item
	{
		start,	//　戦闘開始
		placement,	// 配置換え
		warsituation,	// 戦況確認
		back,
		max
	};

	struct ItemInf
	{
		std::string name;
		Vector2Int pos;
		std::function<void(void)> func;
	};

	Item _selectItem;
	Camera& _camera;
	MapCtrl& _mapCtrl;

	bool _execution;
	bool _backMapSelect;
	static int _itemScreenH;

	void (PreparationUI::* _updater)(const Input& input);

	std::array<ItemInf, static_cast<size_t>(Item::max)> _itemInfTable;

	std::unique_ptr<Track<float>> _animTrack;
	std::unique_ptr<Track<float>> _selectExRateTrack;
	std::unique_ptr<BattlePreparationCursor> _battlePreparationCursor;

	void CloseUpdate(const Input& input);
	void OpenUpdate(const Input& input);
	void CloseAnimUpdate(const Input& input);
	void OpenAnimUpdate(const Input& input);

public:
	PreparationUI(std::deque<std::shared_ptr<UI>>& uiDeque, Camera& camera, MapCtrl& mapCtrl);
	~PreparationUI();

	void Update(const Input& input)override;
	void Draw()override;

	void BeginDraw();

	void Open(const bool animation);
	void Close(const bool animation);

	void SetBackMapSelect(const bool backMapSelect);
	bool GetBackMapSelect();
};