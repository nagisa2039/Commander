#pragma once
#include <vector>
#include <string>
#include "MapChip.h"

class Camera;

/// <summary>
/// �}�b�v���
/// </summary>
class Map
{
public:

private:
	// �}�b�vID
	const int _mapId;
	// �}�b�v��
	const std::string _name;
	// �}�b�v�t�@�C���l�[��
	const std::string _fileName;
	// BGM�t�@�C���l�[��
	const std::string _bgmName;
	// �}�b�v�`�b�v�T�C�Y
	const Size _chipSize;
	// �}�b�v�`�b�v��
	const Size _mapSize;

	// �}�b�v�f�[�^
	std::vector<std::vector<MapData>> _mapDataVec2;

	// �}�b�v�`���X�N���[���n���h��
	int _mapGraphHandle;
	// BGM�n���h��
	int _bgmH;

	// �摜�t�H���_�p�X
	const std::string imageFolderPath;

	/// <summary>
	/// �}�b�v�f�[�^�̓ǂݍ���
	/// </summary>
	/// <returns></returns>
	bool LoadMapData();

	/// <summary>
	/// _mapGraphHandle�ւ̕`��
	/// </summary>
	void DrawToMapScreen();

	/// <summary>
	/// �w�肵���}�X���}�b�v�̕ύX�\�Ȕ͈͂��𒲂ׂ�
	/// </summary>
	/// <param name="mapPos">�m�F������W</param>
	bool CheckMapPosPutRange(const Vector2Int& mapPos);

public:
	/// <param name="mapId">�}�b�vID</param>
	/// <param name="name">�}�b�v��</param>
	/// <param name="fileName">�}�b�v�t�@�C����</param>
	/// <param name="bgmName">BGM�t�@�C����</param>
	Map(const int mapId, std::string& name, std::string& fileName, std::string& bgmName);
	~Map();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J����</param>
	void Draw(const Camera& camera);

	/// <summary>
	/// �}�b�v�`�b�v�̕`��
	/// </summary>
	/// <param name="mapPos">���W</param>
	/// <param name="mapChip">�}�b�v�`�b�v</param>
	/// <param name="offset">�I�t�Z�b�g</param>
	void DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset = Vector2Int(0, 0));

	/// <summary>
	/// �}�b�v���̎擾
	/// </summary>
	const std::string& GetName()const;

	/// <summary>
	/// mapGraphH�̎擾
	/// </summary>
	const int GetMapGraphH()const;

	/// <summary>
	/// �}�b�vID�̎擾
	/// </summary>
	const int GetMapID()const;

	/// <summary>
	/// �}�b�v�`�b�v�T�C�Y�̎擾
	/// </summary>
	const Size& GetChipSize()const;

	/// <summary>
	/// �}�b�v�`�b�v���̎擾
	/// </summary>
	const Size& GetMapSize()const;

	/// <summary>
	/// �}�b�v�̎擾
	/// </summary>
	const std::vector<std::vector<MapData>>& GetMapData()const;

	/// <summary>
	/// �w�肵�����W�̃}�b�v�ް����擾
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	const MapData& GetMapData(const Vector2Int& mapPos)const;

	/// <summary>
	/// �w�肵�����W�̃L�����N�^�[����擾
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	CharactorChipInf GetCharactorChipInf(const Vector2Int& mapPos)const;

	/// <summary>
	///  �w�肵�����W�̃L�����N�^�[���ݒ�
	/// </summary>
	/// <param name="charactorChipInf"></param>
	/// <returns></returns>
	bool SetCharactorChip(const CharactorChipInf& charactorChipInf);

	/// <summary>
	/// �w�肵���}�X���}�b�v�͈͓̔�����Ԃ�
	/// </summary>
	/// <param name="mapPos">�}�b�v���W</param>
	bool CheckMapDataRange(const Vector2Int& mapPos)const;

	/// <summary>
	/// BGM�Đ����J�n����
	/// </summary>
	/// <param name="volume">���� 0�`255</param>
	void StartBGM(const int volume = 255);

	/// <summary>
	///  BGM�Đ����~����
	/// </summary>
	void StopBGM();
};

