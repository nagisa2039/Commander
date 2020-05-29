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

class Camera;
class Charactor;

class SceneController;
class Effect;

class MapCtrl
{
private:
	struct SaveData
	{
		CharactorType charType;
		Status status;
		SaveData():charType(CharactorType::max), status(Status()) {};
		SaveData(const CharactorType ct, const Status& st):charType(ct), status(st) {};
	};
	std::vector<SaveData> _charactorSaveDataVec;

	std::vector<std::vector<Map_Chip>> _mapDataVec2;			// �}�b�v�f�[�^
	std::shared_ptr<Astar> _astar;

	std::vector<std::shared_ptr<Charactor>>& _charactors;

	std::vector<CharactorChipInf> _charactorChips;

	// CharactorType�ɑΉ�����Charactor�N���X��_charactors�ɒǉ�����
	std::array<std::function<void(const CharactorChipInf&, SceneController&, std::vector<std::shared_ptr<Effect>>&, Camera& camera)>,
		static_cast<size_t>(CharactorType::max)> _charactorCreateFuncs;

	int _mapFloorH;
	int _mapChipH;
	int _warSituationH;

	const std::string imageFolderPath;

	void DrawToMapFloorScreen();
	void DrawToMapChipScreen();

public:
	MapCtrl(std::vector<std::shared_ptr<Charactor>>& charactors);
	~MapCtrl();

	void Draw(const Camera& camera, const bool edit = false);

	Size GetChipSize()const;
	Size GetMapSize()const;

	// ���̃}�X�ɂ���L�����N�^�[��Ԃ�
	Charactor* GetMapPosChar(const Vector2Int mapPos)const;

	bool SetMapChip(const Vector2Int& mapPos, const Map_Chip mapChip);
	bool DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset = Vector2Int(0,0));

	bool SetCharactorChip(const CharactorChipInf& charactorChipInf);
	bool DrawCharactorChip(const CharactorChipInf& charactorChipInf, const Vector2Int& offset = Vector2Int(0,0));

	void CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

	bool SaveMap(const std::string fileName);
	bool LoadMap(const std::string fileName);

	// �ړ��\�ȃ}�X��T��
	void RouteSearch(Charactor& charactor);

	// �w�肵���ړ��͂ōs����ꏊ��T��
	bool MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, std::vector<std::vector<std::list<Astar::ResultPos>>>& resultPosListVec2, const Team team);

	// �w��L�������王��͈͂ɂ���L�����N�^�[��Ԃ�
	Vector2Int SearchMovePos(Charactor& charactor);

	void CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2, const Team team);

	// �w�肵�����W��MapChipData���擾����
	DataBase::MapChipData GetMapChipData(const Vector2Int& mapPos)const;

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

	bool CreateCharactorData();
	bool SaveCharactorData();
	bool LoadCharactorData();

	Status GetLevelInitStatus(const uint8_t level, const CharactorType charType);
};