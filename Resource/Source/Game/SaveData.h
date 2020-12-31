#pragma once
#include <vector>
#include <memory>

/// <summary>
/// �Z�[�u�f�[�^�Ǘ��N���X
/// </summary>
class SaveData
{
private:
	// 0�����Ȃ疢�J��
	// 0�Ȃ�J���ς݁A���N���A
	// 1�ȏ�Ȃ�N���A�^�[����
	std::vector<int> _clearMapDataVec;

	/// <summary>
	/// �f�[�^��������
	/// </summary>
	void WriteData();

public:
	SaveData();
	~SaveData();

	/// <summary>
	/// �f�[�^�̃Z�[�u
	/// </summary>
	/// <param name="mapNum">�N���A�����}�b�v�ԍ�</param>
	/// <param name="turnCnt">�N���A�ɗv�����^�[����</param>
	/// <returns>����</returns>
	bool Save(const int mapNum, const int turnCnt);

	/// <summary>
	/// �Z�[�u�f�[�^�̃��[�h
	/// </summary>
	/// <returns>����</returns>
	bool Load();

	/// <summary>
	/// �Z�[�u�f�[�^�̏�����
	/// </summary>
	/// <returns>����</returns>
	bool Reset();

	/// <summary>
	/// �X�e�[�W�̑S�J��
	/// </summary>
	/// <returns>����</returns>
	bool AllOpen();

	/// <summary>
	/// �}�b�v�����̃N���A�󋵂��擾
	/// 0�����Ȃ疢�J��
	/// 0�Ȃ�J����
	/// 1�ȏ�Ȃ�N���A�^�[����
	/// </summary>
	/// <returns></returns>
	const std::vector<int>& GetClearMapDataVec()const;
};

