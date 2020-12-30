#pragma once
#include <vector>
#include <string>
#include <array>
#include "MapChip.h"
#include <memory>
#include "Astar.h"
#include <functional>
#include "DataBase.h"
#include <windows.h>

class Camera;
class Charactor;
class SceneController;
class Effect;
class Map;

/// <summary>
/// �}�b�v�Ǘ��N���X
/// </summary>
class MapCtrl
{
private:
	// 
	std::unique_ptr<Astar> _astar;
	// 
	std::shared_ptr<Map> _map;

	// 
	int _warSituationH;

	// 
	std::vector<std::shared_ptr<Charactor>>& _charactors;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="characotChipInf"></param>
	/// <param name="initStatus"></param>
	/// <param name="ctrl"></param>
	/// <param name="effects"></param>
	/// <param name="camera"></param>
	void CreateCharactor(const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

public:
	/// <param name="mapId"></param>
	/// <param name="charactors"></param>
	MapCtrl(const int mapId, std::vector<std::shared_ptr<Charactor>>& charactors);
	~MapCtrl();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="offset"></param>
	/// <param name="charactorChipInf"></param>
	/// <param name="color"></param>
	/// <param name="frameColor"></param>
	/// <returns></returns>
	bool DrawSortieMass(const Vector2Int& offset, const CharactorChipInf& charactorChipInf, const unsigned int color = 0x00ffff, const unsigned int frameColor = 0x0000ff);

	/// <summary>
	/// ���̃}�X�ɂ���L�����N�^�[��Ԃ�
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	Charactor* GetMapPosChar(const Vector2Int mapPos)const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const Size& GetChipSize()const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const Size& GetMapSize()const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="ctrl"></param>
	/// <param name="effects"></param>
	/// <param name="camera"></param>
	void CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

	/// <summary>
	/// ���[�g�����p�}�b�v�f�[�^�̎擾
	/// </summary>
	/// <param name="mapVec2">���ʊi�[�p���[�g�����p�}�b�v�f�[�^</param>
	void CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2);

	/// <summary>
	/// �틵�m�F�p�̉摜�𐶐�����(�X�V����)
	/// </summary>
	void CreateWarSituation()const;

	/// <summary>
	/// �w�肵�����𒆐S��WarSituation��`�悷��
	/// </summary>
	/// <param name="center">�`�撆�S���W</param>
	void DrawWarSituatin(const Vector2Int center)const;

	/// <summary>
	/// ���R�ƓG�R�̐���Ԃ�(x : ���R, y : �G�R)
	/// </summary>
	Vector2Int GetCharactorCnt()const;

	/// <summary>
	/// ���s���̎��R����Ԃ�
	/// </summary>
	unsigned int GetCanMoveCnt()const;

	/// <summary>
	/// �S�L�����N�^�[�̍s���͈͌v�Z���s��
	/// </summary>
	void AllCharactorRouteSearch()const;

	/// <summary>
	/// �w�肵���W�c��moveActive��ύX
	/// </summary>
	/// <param name="team">�`�[��</param>
	/// <param name="groupNum">�W�c�ԍ�</param>
	/// <param name="active">�L�������邩</param>
	void SetGroupActive(const Team team, const unsigned int groupNum, const bool active);

	/// <summary>
	/// �L�����N�^�[�̔z����擾
	/// </summary>
	const std::vector<std::shared_ptr<Charactor>>& GetCharacots()const;

	/// <summary>
	/// �}�b�v���̎擾
	/// </summary>
	std::shared_ptr<Map> GetMap()const;

	/// <summary>
	/// ���[�g�����I�u�W�F�N�g�̎擾
	/// </summary>
	Astar& GetAstar();
};

