#pragma once
#include <vector>
#include <string>
#include <array>
#include "MapChip.h"
#include <memory>
#include "Astar.h"
#include "CharactorType.h"
#include <functional>

class Camera;
class Charactor;

class SceneController;
class Effect;

class MapCtrl
{
public:
	struct DrawData
	{
		Vector2Int leftup;
		Size size;
		std::string path;

		DrawData() {};

		DrawData(Vector2Int leftup, Size size, std::string path) :
			leftup(leftup), size(size), path(path) {};

	};

	struct MapChipData
	{
		DrawData drawData;
		int moveCost;
		int defense;
		int avoidance;
		const char* name;
		unsigned int simpleColor;

		MapChipData() : drawData(DrawData()), name(""), moveCost(1), defense(0), avoidance(0), simpleColor(0x000000){};
		MapChipData(const DrawData& dd, const char* na, unsigned int sc) :
			drawData(dd), name(na), simpleColor(sc), moveCost(1), defense(0), avoidance(0) {};
		MapChipData(const DrawData& dd, const char* na, unsigned int sc, const int mc) :
			drawData(dd), name(na), simpleColor(sc), moveCost(mc), defense(0), avoidance(0) {};
		MapChipData(const DrawData& dd, const char* na, unsigned int sc, const int mc, const int fe, const int av) :
			drawData(dd), name(na), simpleColor(sc), moveCost(mc), defense(fe), avoidance(av) {};
	};

private:
	std::vector<std::vector<Map_Chip>> _mapDataVec2;			// �}�b�v�f�[�^
	std::array<MapChipData, static_cast<size_t>(Map_Chip::max)> _mapChipData;	// �}�b�v�`�b�v�̃f�[�^
	std::shared_ptr<Astar> _astar;

	std::vector<std::shared_ptr<Charactor>>& _charactors;

	std::array<std::string, static_cast<size_t>(CharactorType::max)> _iconPaths;
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
	Size GetMapCnt()const;

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
	bool MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, std::list<Astar::ResultPos>& resutlPosList, const Team team);

	// �w��L�������王��͈͂ɂ���L�����N�^�[��Ԃ�
	Vector2Int SearchMovePos(Charactor& charactor);

	void CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2, const Team team);

	// �w�肵�����W��MapChipData���擾����
	MapChipData GetMapChipData(const Vector2Int& mapPos)const;

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
};