#pragma once
#include "Cell.h"
#include "Transform.h"
#include "GamePoints.h"
#include <rapidjson\document.h>
#include <array>
class Field
{
public:
	/// <summary>
	/// �t�B�[���h���
	/// </summary>
	_Grid<Cell> cells = { {6,6} };

private:
	/// <summary>
	/// �͂܂�Ă���̈��T������
	/// </summary>
	/// <param name="pos">�T����J�n������W</param>
	/// <param name="tile">�ǂ̃^�C���ň͂܂�Ă��邩</param>
	void dfsAreaPoint(_Point<> pos, TileType tile, _Grid<bool>& _status)const;

	/// <summary>
	/// �w��̃^�C���ň͂܂ꂽ�̈�̓��_��W�v���܂�
	/// </summary>
	/// <param name="tile">�ǂ̃^�C���ň͂܂�Ă��邩</param>
	/// <returns>�̈�|�C���g</returns>
	int aggregateAreaPoint(TileType tile)const;

	/// <summary>
	/// �w��̃^�C���̃^�C���|�C���g��W�v���܂�
	/// </summary>
	/// <param name="tile">���_��W�v����^�C��</param>
	/// <returns>�^�C���|�C���g</returns>
	int aggregateTilePoint(TileType tile)const;
	int aggregateTotalPoint(TileType tile)const;

public:

	std::array<int, 2> GetAreaPoints() const;
	std::array<int, 2> GetTilePoints() const;
	std::array<int, 2> GetTotalPoints() const;
	/// <summary>
	/// �^�C���Ɨ̈�̃|�C���g��W�v���A�f�[�^��X�V����
	/// </summary>
	void UpdatePoint();

	/// <summary>
	/// �Z������擾����
	/// </summary>
	/// <returns>�t�B�[���h�̃Z�����</returns>
	_Grid<Cell> GetCells() const;

	void GetCellsTo(_Grid<Cell>* cells);

	/// <summary>
	/// �Z����h��
	/// </summary>
	/// <param name="pos">�h��Z���̍��W</param>
	/// <param name="team">�Z����h��`�[��</param>
	void PaintCell(_Point<> pos, TeamType team);

	/// <summary>
	/// �^�C������
	/// </summary>
	/// <param name="pos">�^�C���̍��W</param>
	void RemoveTile(_Point<> pos);

	/// <summary>
	/// �w����W���t�B�[���h�̒��ł��邩���肷��
	/// </summary>
	/// <param name="pos">�t�B�[���h�����肷����W</param>
	/// <returns>���W���t�B�[���h��ł��邩</returns>
	bool IsInField(_Point<> pos) const;

	/// <summary>
	/// ���W�ƕ����A�t�B�[���h��Ԃ���K�v�ȍs����߂�
	/// </summary>
	/// <param name="pos">�w������ɓ����O�̍��W</param>
	/// <param name="dir">�s���������</param>
	/// <returns>�K�v�ȍs��</returns>
	Step DecideStepByDirection(_Point<> pos, Direction dir) const;

public:
	static Field makeFieldFromJson(std::string json);
	static Field makeFieldRandom(_Size size = { 6,6 });
};
