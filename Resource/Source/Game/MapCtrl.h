#pragma once
#include <vector>
#include <string>
#include <array>
#include "MapChip.h"
#include <memory>
#include "Astar.h"
#include "CharactorType.h"
#include <functional>
#include "DataBase.h"
#include <windows.h>

class Camera;
class Charactor;
class SceneController;
class Effect;

class MapCtrl
{
private:

	struct MapData
	{
		Map_Chip mapChip;
		CharactorChipInf charactorChip;
	};

	std::vector<std::vector<MapData>> _mapDataVec2;			// �}�b�v�f�[�^
	std::unique_ptr<Astar> _astar;

	std::vector<std::shared_ptr<Charactor>>& _charactors;

	// CharactorType�ɑΉ�����Charactor�N���X��_charactors�ɒǉ�����
	std::array<std::function<void(const CharactorChipInf&, const Status&, SceneController&, std::vector<std::shared_ptr<Effect>>&, Camera&)>,
		static_cast<size_t>(CharactorType::max)> _charactorCreateFuncs;

	int _mapFloorH;
	int _mapChipH;
	int _warSituationH;

	int _mapId;

	const std::string imageFolderPath;

	void DrawToMapFloorScreen();
	void DrawToMapChipScreen();

	bool FileSave(const HWND hWnd, const char* ext, const char* filter, const char* title, std::string& saveFilePath);
	bool FileLoad(const HWND hWnd, const char* ext, const char* filter, const char* title, std::string& loadFilePath);
	void SaveMapData(const std::string& saveFilePath);
	bool LoadMapData(const std::string& filePath);

	// �w�肵���}�X���}�b�v�̕ύX�\�Ȕ͈͉��𒲂ׂ�
	bool CheckMapPosPutRange(const Vector2Int& mapPos); 

	template<typename T>
	inline void CreateCharactor(const CharactorChipInf& characotChipInf, const Status& initStatus,
		SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

public:
	MapCtrl(std::vector<std::shared_ptr<Charactor>>& charactors);
	~MapCtrl();

	void Draw(const Camera& camera, const bool edit = false);

	bool DrawSortieMass(const Vector2Int& offset, const CharactorChipInf& charactorChipInf, const unsigned int color = 0x00ffff, const unsigned int frameColor = 0x0000ff);

	Size GetChipSize()const;
	Size GetMapSize()const;

	// ���̃}�X�ɂ���L�����N�^�[��Ԃ�
	Charactor* GetMapPosChar(const Vector2Int mapPos)const;

	bool SetMapChip(const Vector2Int& mapPos, const Map_Chip mapChip);
	bool DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset = Vector2Int(0,0));

	const std::vector<std::vector<MapData>>& GetMapData()const;
	CharactorChipInf GetCharactorChipInf(const Vector2Int& mapPos)const;
	bool SetCharactorChip(const CharactorChipInf& charactorChipInf);
	bool DrawCharactorChip(const CharactorChipInf& charactorChipInf, const Vector2Int& offset = Vector2Int(0,0));

	void CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

	bool SaveMap();
	bool LoadMap(const int mapId);
	bool LoadMap();

	// �ړ��\�ȃ}�X��T��
	void RouteSearch(Charactor& charactor);

	// �w�肵���ړ��͂ōs����ꏊ��T��
	bool MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, std::list<Astar::ResultPos>& resultPosList, const Team team, const std::list<Astar::ResultPos>& excludeList);

	// �w��L�������王��͈͂ɂ���L�����N�^�[��Ԃ�
	Vector2Int SearchMovePos(Charactor& charactor, Vector2Int& targetCnt);

	void CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2, const Team team);

	// �w�肵�����W��MapData���擾����
	MapData GetMapData(const Vector2Int& mapPos)const;

	// �틵�m�F�p�̉摜�𐶐�����(�X�V����)
	void CreateWarSituation()const;

	// �w�肵�����𒆐S��WarSituation��`�悷��
	void DrawWarSituatin(const Vector2Int center)const;

	// ���R�ƓG�R�̐���Ԃ�(x : ���R, y : �G�R)
	Vector2Int GetCharactorCnt()const;

	// ���s���̎��R����Ԃ�
	unsigned int GetCanMoveCnt()const;

	// �S�L�����N�^�[�̍s���͈͌v�Z���s��
	void AllCharactorRouteSearch()const;

	// �w�肵���W�c��moveActive��ύX
	void SetGroupActive(const unsigned int groupNum, const bool active);

	// �w�肵���}�X���}�b�v�͈͓̔�����Ԃ�
	bool CheckMapDataRange(const Vector2Int& mapPos);

	// �L�����N�^�[�̔z����擾
	const std::vector<std::shared_ptr<Charactor>>& GetCharacots()const;

	// ���݂̃}�b�vID��Ԃ�
	const int GetMapID()const;
};

template<typename T>
inline void MapCtrl::CreateCharactor(const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
{
	auto charactor = make_shared<T>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera);
	charactor->SetMoveActive(characotChipInf.active);
	charactor->InitStatus(initStatus);
	_charactors.emplace_back(charactor);
}
