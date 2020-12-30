#pragma once
#include <vector>
#include <list>
#include "Dir.h"
#include "Geometry.h"
#include "Astar.h"

class Charactor;
class MapCtrl;
class Camera;

/// <summary>
/// �o�H�Ǘ��N���X
/// </summary>
class RouteManager
{
private:
	/// <summary>
	/// �ړ����
	/// </summary>
	struct MoveInf
	{
		// ����
		Dir dir;
		// �U���}�X��
		bool attack;
		// �}�X��
		Vector2Int mapPos;
	};

	// �o�H�z��
	std::vector<std::vector<std::list<Astar::ResultPos>>> _resultPosListVec2;
	// �ړ������z��
	std::list<MoveInf> _moveDirList;

	// �s���͈͓��̃L�����N�^�[���i�[
	std::list<Charactor*> _inRangeCharactorList;

	// ���L�҂̃L�����N�^�[
	Charactor& _charactor;
	// �}�b�v�Ǘ��N���X
	MapCtrl& _mapCtrl;
	// �J����
	Camera& _camera;

	/// <summary>
	/// �w�肵�����W�ɉ��̃}�X�����邩���ׂ�	
	/// </summary>
	/// <param name="mapPos">�}�X��</param>
	/// <returns>x=�ړ��}�X y=�U���}�X</returns>
	Vector2Int CheckMapPos(const Vector2Int& mapPos)const;

public:
	/// <param name="charactor">���L�҂̃L�����N�^�[</param>
	/// <param name="mapCtrl">�}�b�v�Ǘ��N���X</param>
	/// <param name="camera">�J����</param>
	RouteManager(Charactor& charactor, MapCtrl& mapCtrl, Camera& camera);
	~RouteManager();

	/// <summary>
	/// �ړ��͈͂̕`��
	/// </summary>
	/// <param name="alpha">�s�����x</param>
	void DrawMovableMass(const uint8_t alpha)const;

	/// <summary>
	/// �w�肵�����W�ɍU���\�ȃL�����N�^�[�����邩�m�F
	/// �����ꍇ�͍U���J�n���W���擾
	/// </summary>
	/// <param name="attackPos">�U���J�n���W���擾���邽�߂̕ϐ�</param>
	/// <param name="targetMapPos">�m�F������W</param>
	bool GetAttackStartPos(Vector2Int& attackPos, const Vector2Int& targetMapPos)const;

	/// <summary>
	/// �o�H�T���̌��ʂ��i�[�����z��擾
	/// </summary>
	std::vector<std::vector<std::list<Astar::ResultPos>>>& GetResutlPosListVec2();

	/// <summary>
	/// �ړ������z��̎擾
	/// </summary>
	std::list<MoveInf>& GetMoveDirList();

	// �w�肵�����W�܂ł̌o�H��`�悷��
	void DrawRoute(const Vector2Int& targetPos);
	// ���݂̏ꏊ����U���ł��鑊��̍��W��Ԃ�
	std::list<Vector2Int> GetAttackPosList()const;

	/// <summary>
	/// �w�肵���}�X�܂ł�ResultPosList���擾����
	/// </summary>
	std::list<Astar::ResultPos> CreateResultPosList(const Vector2Int& mapPos)const;

	/// <summary>
	/// resultPosList����MoveDirList���쐬����
	/// </summary>
	/// <param name="resultPosList">��ɂ���ResultPosList</param>
	void CreateMoveDirList(const std::list<Astar::ResultPos>& resultPosList);

	/// <summary>
	/// �͈͓��̃^�[�Q�b�g�L�����N�^�[���W�߂�
	/// </summary>
	void SearchRangeTarget();

	/// <summary>
	/// �s���͈͓��Ƀ^�[�Q�b�g�̃L�����N�^�[�����邩��Ԃ�
	/// </summary>
	bool CheckInRageTarget();

	/// <summary>
	/// �ړ��\�ȃ}�X��T��
	/// </summary>
	void RouteSearch();

	/// <summary>
	/// �w�肵���ړ��͂ōs����ꏊ��T��
	/// </summary>
	/// <param name="startPos">�����J�n�}�X</param>
	/// <param name="move">�ړ���</param>
	/// <param name="resultPosList">���ʊi�[�ϐ�</param>
	/// <param name="team">�`�[��</param>
	/// <param name="excludeList">���O����}�X���X�g</param>
	/// <returns>���݂��邩</returns>
	bool MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, 
		std::list<Astar::ResultPos>& resultPosList, const Team team, const std::list<Astar::ResultPos>& excludeList);

	/// <summary>
	/// 
	/// </summary>
	Vector2Int SearchMovePos();
};

