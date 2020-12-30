#pragma once
#include <vector>
#include <string>
#include "MapChip.h"

class Camera;

/// <summary>
/// マップ情報
/// </summary>
class Map
{
public:

private:
	// マップID
	const int _mapId;
	// マップ名
	const std::string _name;
	// マップファイルネーム
	const std::string _fileName;
	// BGMファイルネーム
	const std::string _bgmName;
	// マップチップサイズ
	const Size _chipSize;
	// マップチップ数
	const Size _mapSize;

	// マップデータ
	std::vector<std::vector<MapData>> _mapDataVec2;

	// マップ描画先スクリーンハンドル
	int _mapGraphHandle;
	// BGMハンドル
	int _bgmH;

	// 画像フォルダパス
	const std::string imageFolderPath;

	/// <summary>
	/// マップデータの読み込み
	/// </summary>
	/// <returns></returns>
	bool LoadMapData();

	/// <summary>
	/// _mapGraphHandleへの描画
	/// </summary>
	void DrawToMapScreen();

	/// <summary>
	/// 指定したマスがマップの変更可能な範囲かを調べる
	/// </summary>
	/// <param name="mapPos">確認する座標</param>
	bool CheckMapPosPutRange(const Vector2Int& mapPos);

public:
	/// <param name="mapId">マップID</param>
	/// <param name="name">マップ名</param>
	/// <param name="fileName">マップファイル名</param>
	/// <param name="bgmName">BGMファイル名</param>
	Map(const int mapId, std::string& name, std::string& fileName, std::string& bgmName);
	~Map();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(const Camera& camera);

	/// <summary>
	/// マップチップの描画
	/// </summary>
	/// <param name="mapPos">座標</param>
	/// <param name="mapChip">マップチップ</param>
	/// <param name="offset">オフセット</param>
	void DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset = Vector2Int(0, 0));

	/// <summary>
	/// マップ名の取得
	/// </summary>
	const std::string& GetName()const;

	/// <summary>
	/// mapGraphHの取得
	/// </summary>
	const int GetMapGraphH()const;

	/// <summary>
	/// マップIDの取得
	/// </summary>
	const int GetMapID()const;

	/// <summary>
	/// マップチップサイズの取得
	/// </summary>
	const Size& GetChipSize()const;

	/// <summary>
	/// マップチップ数の取得
	/// </summary>
	const Size& GetMapSize()const;

	/// <summary>
	/// マップの取得
	/// </summary>
	const std::vector<std::vector<MapData>>& GetMapData()const;

	/// <summary>
	/// 指定した座標のマップﾃﾞｰﾀを取得
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	const MapData& GetMapData(const Vector2Int& mapPos)const;

	/// <summary>
	/// 指定した座標のキャラクター種を取得
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	CharactorChipInf GetCharactorChipInf(const Vector2Int& mapPos)const;

	/// <summary>
	///  指定した座標のキャラクター種を設定
	/// </summary>
	/// <param name="charactorChipInf"></param>
	/// <returns></returns>
	bool SetCharactorChip(const CharactorChipInf& charactorChipInf);

	/// <summary>
	/// 指定したマスがマップの範囲内かを返す
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	bool CheckMapDataRange(const Vector2Int& mapPos)const;

	/// <summary>
	/// BGM再生を開始する
	/// </summary>
	/// <param name="volume">音量 0～255</param>
	void StartBGM(const int volume = 255);

	/// <summary>
	///  BGM再生を停止する
	/// </summary>
	void StopBGM();
};

