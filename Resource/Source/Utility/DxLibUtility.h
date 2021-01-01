#pragma once
#include "Geometry.h"
#include "AnkerCalculation.h"

/// <summary>
/// �摜�T�C�Y�擾
/// </summary>
/// <param name="handle">�摜�n���h��</param>
/// <param name="size">�摜�T�C�Y�i�[�p�ϐ�</param>
void GetGraphSize(const int handle, Size& size);

/// <summary>
/// ��`�`��
/// </summary>
/// <param name="leftup">������W</param>
/// <param name="rightdown">�E�����W</param>
/// <param name="color">�F</param>
/// <param name="fillFlag">�h��Ԃ���</param>
void DrawBox(const Vector2Int& leftup, const Vector2Int& rightdown, const unsigned int color, const bool fillFlag = true);

/// <summary>
/// �����̕`��
/// </summary>
/// <param name="start">�n�_</param>
/// <param name="end">�I�_</param>
/// <param name="color">�F</param>
/// <param name="thickness">����</param>
void DrawLine(const Vector2Int& start, const Vector2Int& end, const unsigned int color, const int thickness = 1);

/// <summary>
/// �摜�`��
/// </summary>
/// <param name="leftup">����</param>
/// <param name="handle">�n���h��</param>
/// <param name="transFlag">�����x��L�������邩</param>
void DrawGraph(const Vector2Int& leftup, const int handle, const bool transFlag = true);

/// <summary>
/// �摜��]�`��
/// </summary>
/// <param name="center">���S���W</param>
/// <param name="ExRate">�g�嗦</param>
/// <param name="Angle">��](���W�A��)</param>
/// <param name="GrHandle">�n���h��</param>
/// <param name="TransFlag">�����x��L�������邩</param>
/// <param name="ReverseXFlag">X�����]���邩</param>
/// <param name="ReverseYFlag">Y�����]���邩</param>
void DrawRotaGraph(const Vector2Int& center, const double ExRate, const double Angle, const int GrHandle, const int TransFlag, const int ReverseXFlag = false, const int ReverseYFlag = false);

/// <summary>
/// �摜��]�؂蔲���`��
/// </summary>
/// <param name="center">���S</param>
/// <param name="srcLeftup">�؂蔲����`�̍���</param>
/// <param name="size">�؂蔲���T�C�Y</param>
/// <param name="ExtRate">�g�嗦</param>
/// <param name="Angle">��](���W�A��)</param>
/// <param name="GrHandle">�n���h��</param>
/// <param name="TransFlag">�����x��L�������邩</param>
/// <param name="ReverseXFlag">X�����]���邩</param>
/// <param name="ReverseYFlag">Y�����]���邩</param>
void DrawRectRotaGraph(const Vector2Int& center, const Vector2Int& srcLeftup, const Size& size, const double ExtRate, const double Angle, const int GraphHandle, const int TransFlag = true, const int ReverseXFlag = false, const int ReverseYFlag = false);

/// <summary>
/// �~��`��
/// </summary>
/// <param name="center">���S</param>
/// <param name="radius">���a</param>
/// <param name="color">�F</param>
/// <param name="fillFlag">�h��Ԃ���</param>
void DrawCircle(const Vector2Int& center, const int radius, const unsigned int color, const bool fillFlag = true);

/// <summary>
/// �`����W(������W)�̎擾
/// ��: �E��(�A���J�[�w��)���W(drawPos�w��)��������W�֕ϊ�����
/// </summary>
/// <param name="drawPos">�`����W</param>
/// <param name="size">�T�C�Y</param>
/// <param name="anker">�A���J�[</param>
Vector2Int GetDrawPos(const Vector2Int& drawPos, const Size& size, const Anker anker);

/// <summary>
/// �t�H���g���g�p����������`��
/// </summary>
/// <param name="drawPos">�`����W</param>
/// <param name="anker">�A���J�[</param>
/// <param name="color">�F</param>
/// <param name="fontHandle">�t�H���g�n���h��</param>
/// <param name="format">������t�H�[�}�b�g</param>
void DrawStringToHandle(const Vector2Int& drawPos, const Anker anker, const unsigned color, const int fontHandle, const char* format, ...);

/// <summary>
/// �t�H���g���g�p����������`��
/// </summary>
/// <param name="drawPos">�`����W</param>
/// <param name="anker">�A���J�[</param>
/// <param name="color">�F</param>
/// <param name="exRate">�g�嗦</param>
/// <param name="fontHandle">�t�H���g�n���h��</param>
/// <param name="format">������t�H�[�}�b�g</param>
void DrawStringToHandle(const Vector2Int& drawPos, const Anker anker, const unsigned color, const double exRate, const int fontHandle, const char* format, ...);