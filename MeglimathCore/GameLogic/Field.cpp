#include "Field.h"
#include <time.h>
#include<boost/range/irange.hpp>
auto step(int s) { return boost::irange(0, s); }

int Field::aggregateAreaPoint(TileType tile)const
{
	auto _status = _Grid<bool>(cells.size() + _Point<int>(2, 2));

	dfsAreaPoint(_Point<int>(0, 0), tile, _status);

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


void Field::dfsAreaPoint(_Point<> pos, TileType tile, _Grid<bool>& _status)const
{
	// 範囲外なら終了
	if (pos.x < 0 || pos.x > cells.width() + 1
		|| pos.y < 0 || pos.y > cells.height() + 1)
	{
		return;
	}

	// 探索済みなら終了
	if (_status[pos.y][pos.x] == true)
	{
		return;
	}

	_status[pos.y][pos.x] = true;
	if (pos.x == 0 || pos.x == cells.width() + 1 ||
		pos.y == 0 || pos.y == cells.height() + 1)
	{
		// 端は探索のみ行う
	}
	else if (cells[pos.y - 1][pos.x - 1].tile == tile)
	{
		// 調査中のタイルが置かれていたら終了
		return;
	}


	// 四方へ探索する
	for (auto delta : { _Point<>{0, 1},_Point<>{1, 0}, _Point<>{0, -1}, _Point<>{-1, 0} })
	{
		dfsAreaPoint(pos + delta, tile, _status);
	}
}

int Field::aggregateTilePoint(TileType tile)const
{
	int sum_tile_point = 0;

	//	タイルの種類が一致するセルの得点の合計を計算する
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
	return { aggregateAreaPoint(TileType::Blue),aggregateAreaPoint(TileType::Red) };
}

std::array<int, 2> Field::GetTilePoints() const
{
	return { aggregateTilePoint(TileType::Blue),aggregateTilePoint(TileType::Red) };
}

std::array<int, 2> Field::GetTotalPoints() const
{
	return { aggregateTotalPoint(TileType::Blue),aggregateTotalPoint(TileType::Red) };
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

	// 座標から指定の方向に進んだ後の座標
	_Point<int> next_pos = pos + Transform::DirToDelta(dir);

	if (!IsInField(next_pos))
	{
		return Step{ Action::Stop, Direction::Stop };
	}

	// 進んだ先のタイルの有無でアクションを決める
	if (cells[next_pos.y][next_pos.x].tile == TileType::None)
	{
		return Step{ Action::Move, dir };
	}
	else
	{
		return Step{ Action::RemoveTile, dir };
	}
}

Field Field::MakeFieldFromStep(TeamType team, Agent agent, Step step)
{
	Field field = *this;
	auto pos = agent.Moved(step.direction).position;

	switch (step.action)
	{
	case Action::RemoveTile:
		if (!field.CanRemoveTile(pos, team))
		{
			return *this;
		}
		
		field.cells[pos.y][pos.x].RemoveTile();
		return field;

	case Action::Move:
		if (!field.CanMove(pos, team))
		{
			return *this;
		}

		field.cells[pos.y][pos.x].PaintedBy(team);
		return field;
	}

}

bool Field::CanMove(_Point<> pos, TeamType team)
{
	auto other_tile = Transform::GetInverseTile(Transform::ToTile(team));

	return IsInField(pos) && cells[pos.y][pos.x].tile != other_tile;
}

bool Field::CanRemoveTile(_Point<> pos, TeamType team)
{
	return IsInField(pos) && cells[pos.y][pos.x].tile != TileType::None;
}

Field Field::makeFieldFromJson(std::string json)
{
	rapidjson::Document document;
	document.Parse(json.data());
	_Size size = _Size{ document["Size"].GetString() };
	auto points = document["Points"].GetArray();

	auto cells = _Grid<Cell>(size);

	// タイルポイントをグリッド状に成型して入力
	int idx = 0;
	for (int i : step(size.y))
	{
		for (int k : step(size.x))
		{
			cells[i][k] = { points[idx].GetInt() };
			
			idx++;
		}
	}
	if (!document.HasMember("Tiles"))return Field{ cells };

	// テスト用にタイル情報がある場合読み込んで入力する
	auto tiles = document["Tiles"].GetArray();
	for (int i : step(size.y))
	{
		for (int k : step(size.x))
		{
			switch (tiles[i].GetString()[k])
			{
			case 'a':
				cells[i][k].PaintedBy(TeamType::Blue);
				break;
			case 'b':
				cells[i][k].PaintedBy(TeamType::Red);
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
	// 入力されるタイルポイントの数
	_Size data_size = _Size((size.x + 1) / 2, (size.y + 1) / 2);
	cells = _Grid<Cell>(size);
	srand(time(nullptr));
	for (int i : step(data_size.y)) {
		for (int k : step(data_size.x)) {
			cells[i][k] = { (rand() >> 7) % 10 != 0 ?
				(rand() >> 7) % 17 :
				-((rand() >> 7) % 17)
			};
			// データをコピー
			cells[size.y - 1 - i][size.x - 1 - k] = cells[i][k];
			cells[size.y - 1 - i][k] = cells[i][k];
			cells[i][size.x - 1 - k] = cells[i][k];
		}
	}
	return { cells };
}

bool Field::IsSameStateField(const Field & other) const
{
	auto s1 = this->cells.size();
	auto s2 = other.cells.size();

	if (s1.x != s2.x || s1.y != s2.y)
	{
		return false;
	}

	auto& size = s1;
	for (auto x = 0; x < size.x; x++)
	{
		for (auto y = 0; y < size.y; y++)
		{
			if (this->cells[y][x].tile != other.cells[y][x].tile)
			{
				return false;
			}
		}
	}

	return true;
}
