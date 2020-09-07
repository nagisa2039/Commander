#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include <array>
#include <functional>

class BattlePreparationCursor;
class MapCtrl;
class Camera;
class PlayScene;

class PreparationUI :
	public UI
{
private:
	enum class Item
	{
		start,	//　戦闘開始
		placement,	// 配置換え
		warsituation,	// 戦況確認
		//shop,		// 店
		back,
		max
	};

	struct ItemInf
	{
		std::string name;
		Vector2Int pos;
		std::function<void(void)> func;
		int graphH = -1;
	};

	Size _itemSize;
	Item _selectItem;

	PlayScene& _playScene;
	Camera& _camera;
	MapCtrl& _mapCtrl;

	/// <summary>
	/// SEhandle系
	/// </summary>
	int _openH;
	int _closeH;
	int _moveH;
	int _okH;
	int _canselH;
	int _bgmH;

	bool _execution;
	/// <summary>
	/// マップ選択に戻ってよいかのフラグ
	/// </summary>
	bool _backMapSelect;

	void (PreparationUI::* _updater)(const Input& input);

	std::array<ItemInf, static_cast<size_t>(Item::max)> _itemInfTable;

	std::unique_ptr<Track<float>> _animTrack;
	std::unique_ptr<Track<float>> _selectExRateTrack;
	std::unique_ptr<BattlePreparationCursor> _battlePreparationCursor;

	void CloseUpdate(const Input& input);
	void OpenUpdate(const Input& input);
	void CloseAnimUpdate(const Input& input);
	void OpenAnimUpdate(const Input& input);

	void CloseEnd();

public:
	PreparationUI(std::deque<std::shared_ptr<UI>>* uiDeque, Camera& camera, MapCtrl& mapCtrl, PlayScene& playScene);
	~PreparationUI();

	void Update(const Input& input)override;
	void Draw()override;

	void BeginDraw();

	void Open(const bool animation);
	void Close(const bool animation);

	// マップ選択に戻るためのフラグセット
	void BackMapSelect();
	// マップ選択に戻るかのフラグ取得
	bool GetBackMapSelect();
};