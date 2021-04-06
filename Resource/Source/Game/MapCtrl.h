#pragma once
#include <vector>
#include <string>
#include <array>
#include "MapChip.h"
#include <memory>
#include <functional>
#include "DataBase.h"
#include <windows.h>
#include "RouteSearchData.h"

class Camera;
class Charactor;
class SceneController;
class Effect;
class Map;
class RouteSearch;

/// <summary>
/// �}�b�v�Ǘ��N���X
/// </summary>
class MapCtrl
{
private:
	// ���[�g�����N���X
	std::unique_ptr<RouteSearch> _routeSearch;
	// �}�b�v���
	std::shared_ptr<Map> _map;

	// �틵�m�F��ʃn���h��
	int _warSituationH;

	// �L�����N�^�[�Ǘ��z��
	std::vector<std::shared_ptr<Charactor>>& _charactors;

	/// <summary>
	/// �L�����N�^�[�̐���
	/// </summary>
	/// <param name="characotChipInf">�L�����N�^�[���</param>
	/// <param name="initStatus">�����X�e�[�^�X</param>
	/// <param name="ctrl">�V�[���Ǘ��N���X</param>
	/// <param name="effects">�G�t�F�N�g�Ǘ��z��</param>
	/// <param name="camera">�J����</param>
	void CreateCharactor(const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

public:
	/// <param name="mapId">�}�b�vID</param>
	/// <param name="charactors">�L�����N�^�[�Ǘ��z��</param>
	MapCtrl(const int mapId, std::vector<std::shared_ptr<Charactor>>& charactors);
	~MapCtrl();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J����</param>
	void Draw(const Camera& camera);

	/// <summary>
	/// �o���\�}�X�̕`��
	/// </summary>
	/// <param name="offset">�`��I�t�Z�b�g</param>
	/// <param name="charactorChipInf">�L�����N�^�[�`�b�v���</param>
	/// <param name="color">�F</param>
	/// <param name="frameColor">�t���[���F</param>
	/// <returns>�v���C���[�}�X��</returns>
	bool DrawSortieMass(const Vector2Int& offset, const CharactorChipInf& charactorChipInf, const unsigned int color = 0x00ffff, const unsigned int frameColor = 0x0000ff);

	/// <summary>
	/// ���̃}�X�ɂ���L�����N�^�[��Ԃ�
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	Charactor* GetMapPosChar(const Vector2Int mapPos)const;

	/// <summary>
	/// �}�b�v�`�b�v�T�C�Y�̎擾
	/// </summary>
	const Size& GetChipSize()const;

	/// <summary>
	/// �}�b�v�`�b�v���̎擾
	/// </summary>
	const Size& GetMapSize()const;

	/// <summary>
	/// �L�����N�^�[�̐���
	/// </summary>
	/// <param name="ctrl">�V�[���Ǘ��N���X</param>
	/// <param name="effects">�G�t�F�N�g�Ǘ��z��</param>
	/// <param name="camera">�J����</param>
	void CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

	/// <summary>
	/// ���[�g�����p�}�b�v�f�[�^�̎擾
	/// </summary>
	/// <param name="mapVec2">���ʊi�[�p���[�g�����p�}�b�v�f�[�^</param>
	void CreateMapVec(SearchData::SearchMapData& mapVec2);

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
	RouteSearch& GetRouteSerch();
};

