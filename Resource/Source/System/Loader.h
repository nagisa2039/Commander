#pragma once
#include <vector>
#include <map>
#include "../Utility/Geometry.h"

//�f�[�^���N���X
class Data
{
public:
	//���̃f�[�^���g�p�\���ǂ�����Ԃ�
	virtual bool IsAvailable()=0;
	virtual ~Data();
};

class Loader
{
public:
	Loader();
	virtual ~Loader();

	//���[�h
	//@param path �t�@�C���p�X
	//@param data data�ւ̎Q��
	//@retcal ���[�h�̐���
	virtual bool Load(const char* path, Data& data)=0;

	//���[�h�f�[�^�̊J��
	//@param path �t�@�C���p�X
	//@attention ����Ŏw�肳�ꂽ�p�X�ɑΉ�����f�[�^��
	//�������ォ��Ȃ��Ȃ�܂��̂�
	// ������x�g�p����Ƃ��ɂ̓X�g���[�W�A�N�Z�X��
	//�������܂�
	virtual void Unload(const char* path)=0;
};

