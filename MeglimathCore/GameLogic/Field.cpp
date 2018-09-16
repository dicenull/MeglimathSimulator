#include "Field.h"
#include <time.h>
#include<boost/range/irange.hpp>
auto step(int s) { return boost::irange(0, s); }

int Field::aggregateAreaPoint(TileType tile)const
{
	auto _status = _Grid<bool>(cells.size() + _Point<int>(2, 2));

	dfsAreaPoint(_Point<int>(0, 0), tile,_status);

	int area_point = 0;
	for (size_t i : step(_status.width()))
	{
		for (size_t k : step(_status.height()))
		{
			if (_status[k][i])
			{
				continue;
			}

			auto cell = cells[k - 1][i - 1];
			if (cell.tile == tile)
			{
				continue;
			}

			area_point += std::abs(cell.point);
		}
	}

	return area_point;
}


void Field::dfsAreaPoint(_Point<> pos, TileType tile,_Grid<bool>& _status)const
{
	// �͈͊O�Ȃ�I��
	if (pos.x < 0 || pos.x > cells.width() + 1
		|| pos.y < 0 || pos.y > cells.height() + 1)
	{
		return;
	}

	// �T���ς݂Ȃ�I��
	if (_status[pos.y][pos.x] == true)
	{
		return;
	}

	_status[pos.y][pos.x] = true;
	if (pos.x == 0 || pos.x == cells.width() + 1 ||
		pos.y == 0 || pos.y == cells.height() + 1)
	{
		// �[�͒T���̂ݍs��
	}
	else if (cells[pos.y - 1][pos.x - 1].tile == tile)
	{
		// �������̃^�C�����u����Ă�����I��
		return;
	}


	// �l���֒T������
	for (auto delta : { _Point<>{0, 1},_Point<>{1, 0}, _Point<>{0, -1}, _Point<>{-1, 0} })
	{
		dfsAreaPoint(pos + delta, tile,_status);
	}
}

int Field::aggregateTilePoint(TileType tile)const
{
	int sum_tile_point = 0;

	//	�^�C���̎�ނ���v����Z���̓��_�̍��v��v�Z����
	for (size_t i : step(cells.width()))
	{
		for (size_t k : step(cells.height()))
		{
			if (cells[k][i].tile == tile)
			{
				sum_tile_point += cells[k][i].point;
			}
		}
	}

	return sum_tile_point;
}

int Field::aggregateTotalPoint(TileType tile)const
{
	return aggregateAreaPoint(tile) + aggregateTilePoint(tile);
}

std::array<int, 2> Field::GetAreaPoints() const
{
	return { aggregateAreaPoint(TileType::A),aggregateAreaPoint(TileType::B) };
}

std::array<int, 2> Field::GetTilePoints() const
{
	return { aggregateTilePoint(TileType::A),aggregateTilePoint(TileType::B) };
}

std::array<int, 2> Field::GetTotalPoints() const
{
	return { aggregateTotalPoint(TileType::A),aggregateTotalPoint(TileType::B) };
}

void Field::PaintCell(_Point<> pos, TeamType team)
{
	cells[pos.y][pos.x].PaintedBy(team);
}

void Field::RemoveTile(_Point<> pos)
{
	cells[pos.y][pos.x].RemoveTile();
}

bool Field::IsInField(_Point<> pos) const
{
	return (0 <= pos.x && pos.x < cells.width()) && (0 <= pos.y && pos.y < cells.height());
}

Step Field::DecideStepByDirection(_Point<> pos, Direction dir) const
{
	if (dir == Direction::Stop)
	{
		return Step{ Action::Stop, Direction::Stop };
	}

	// ���W����w��̕����ɐi�񂾌�̍��W
	_Point<int> next_pos = pos + Transform::DirToDelta(dir);

	if (!IsInField(next_pos))
	{
		return Step{ Action::Stop, Direction::Stop };
	}

	// �i�񂾐�̃^�C���̗L���ŃA�N�V������߂�
	if (cells[next_pos.y][next_pos.x].tile == TileType::None)
	{
		return Step{ Action::Move, dir };
	}
	else
	{
		return Step{ Action::RemoveTile, dir };
	}
}

Field Field::makeFieldFromJson(std::string json)
{
	rapidjson::Document document;
	document.Parse(json.data());
	_Size size = _Size{ document["Size"].GetString() };
	auto points = document["Points"].GetArray();

	// ���͂����^�C���|�C���g�̐�
	_Size data_size = _Size((size.x + 1) / 2, (size.y + 1) / 2);

	auto cells = _Grid<Cell>(size);

	// �^�C���|�C���g��O���b�h��ɐ��^���ē���
	int idx = 0;
	for (int i : step(data_size.y))
	{
		for (int k : step(data_size.x))
		{
			cells[i][k] = { points[idx].GetInt() };
			// �f�[�^��R�s�[
			cells[size.y - 1 - i][size.x - 1 - k] = cells[i][k];
			cells[size.y - 1 - i][k] = cells[i][k];
			cells[i][size.x - 1 - k] = cells[i][k];

			idx++;
		}
	}
	if (!document.HasMember("Tiles"))return Field{ cells };

	// �e�X�g�p�Ƀ^�C����񂪂���ꍇ�ǂݍ���œ��͂���
	auto tiles = document["Tiles"].GetArray();
	for (int i : step(size.y))
	{
		for (int k : step(size.x))
		{
			switch (tiles[i].GetString()[k])
			{
			case 'a':
				cells[i][k].PaintedBy(TeamType::A);
				break;
			case 'b':
				cells[i][k].PaintedBy(TeamType::B);
				break;
			default:
				break;
			}
		}
	}
	return Field{ cells };
}
Field Field::makeFieldRandom(_Size size)
{
	auto cells = _Grid<Cell>(size);
	// ���͂����^�C���|�C���g�̐�
	_Size data_size = _Size((size.x + 1) / 2, (size.y + 1) / 2);
	cells = _Grid<Cell>(size);
	srand(time(nullptr));
	for (int i : step(data_size.y)) {
		for (int k : step(data_size.x)) {
			cells[i][k] = { (rand() >> 7) % 10 != 0 ?
				(rand() >> 7) % 17 :
				-((rand() >> 7) % 17)
			};
			// �f�[�^��R�s�[
			cells[size.y - 1 - i][size.x - 1 - k] = cells[i][k];
			cells[size.y - 1 - i][k] = cells[i][k];
			cells[i][size.x - 1 - k] = cells[i][k];
		}
	}
	return { cells };
}
