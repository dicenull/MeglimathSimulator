#include "Field.h"
#include <time.h>
#include<boost/range/irange.hpp>
auto step(int s) { return boost::irange(0, s); }

int Field::aggregateAreaPoint(TileType tile)const
{
	auto area_number = _Grid<int>(cells.size());	/*-1:タイル*/
	std::array<int, 12 * 12 / 2> area_relation;
	std::array<bool, 12 * 12 / 2> area_wall;
	int n = 0;
	bool empty_continue = false;
	area_relation[0] = 0;
	int w = area_number.width(), h = area_number.height();
	for (int i = 0; i < area_relation.size(); i++) { area_relation[i] = i; area_wall[i] = false; }
	for (size_t i = 0; i < w; i++)
	{
		for (size_t k = 0; k < h; k++)
		{
			int number;
			if (cells[{i,k}].tile != tile) {//空白領域
				if (!empty_continue) {
					n++;
					empty_continue = true;
				}
				if (i != 0) {
					if (area_number[{i - 1,k}] != -1) {//上列が領域なので、領域関係の計算
						int nn = area_number[{i - 1,k}];
						while (area_relation[nn] != nn) {
							int t = nn;
							nn = area_relation[nn];
							area_relation[t] = n;
						}
						if (area_wall[nn])area_wall[n] = true;
						area_relation[nn] = n;
					}
				}
				if ((i == 0) || (k == 0) ||
					(i == area_number.width() - 1) ||
					(k == area_number.height() - 1)) {
					area_wall[n] = true;
				}
				number = n;
			}
			else {//タイル
				empty_continue = false;
				number = -1;
			}
			area_number[{i,k}] = number;

		}
		empty_continue = false;
	}

	int area_point = 0;
	for (size_t i = 0; i < w; i++)
	{
		for (size_t k = 0; k < h; k++)
		{
			int nn = area_number[{i,k}];
			if (nn == -1)continue;
			while (area_relation[nn] != nn) {
				nn = area_relation[nn];
			}
			if (!area_wall[nn])area_point += std::abs(cells[{i, k}].point);
		}
	}

	return area_point;
}


//void Field::dfsAreaPoint(_Point<> pos, TileType tile, _Grid<bool>& _status)const
//{
//	// 範囲外なら終了
//	if (pos.x < 0 || pos.x > cells.width() + 1
//		|| pos.y < 0 || pos.y > cells.height() + 1)
//	{
//		return;
//	}
//
//	// 探索済みなら終了
//	if (_status[{pos.y, pos.x}] == true)
//	{
//		return;
//	}
//
//	_status[{pos.y, pos.x}] = true;
//	if (pos.x == 0 || pos.x == cells.width() + 1 ||
//		pos.y == 0 || pos.y == cells.height() + 1)
//	{
//		// 端は探索のみ行う
//	}
//	else if (cells[{pos.y - 1, pos.x - 1}].tile == tile)
//	{
//		// 調査中のタイルが置かれていたら終了
//		return;
//	}
//
//
//	// 四方へ探索する
//	for (auto delta : { _Point<>{0, 1},_Point<>{1, 0}, _Point<>{0, -1}, _Point<>{-1, 0} })
//	{
//		dfsAreaPoint(pos + delta, tile, _status);
//	}
//}

int Field::aggregateTilePoint(TileType tile)const
{
	int sum_tile_point = 0;

	//	タイルの種類が一致するセルの得点の合計を計算する
	for (size_t i : step(cells.width()))
	{
		for (size_t k : step(cells.height()))
		{
			if (cells[{i, k}].tile == tile)
			{
				sum_tile_point += cells[{i, k}].point;
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
	cells[pos].PaintedBy(team);
}

void Field::RemoveTile(_Point<> pos)
{
	cells[pos].RemoveTile();
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
	if (cells[next_pos].tile == TileType::None)
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

		field.cells[pos].RemoveTile();
		return field;

	case Action::Move:
		if (!field.CanMove(pos, team))
		{
			return *this;
		}

		field.cells[pos].PaintedBy(team);
		return field;
	}

}

bool Field::CanMove(_Point<> pos, TeamType team)
{
	auto other_tile = Transform::GetInverseTile(Transform::ToTile(team));

	return IsInField(pos) && cells[pos].tile != other_tile;
}

bool Field::CanRemoveTile(_Point<> pos, TeamType team)
{
	return IsInField(pos) && cells[pos].tile != TileType::None;
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
	for (size_t i : step(size.y))
	{
		for (size_t k : step(size.x))
		{
			cells[{k, i}] = { points[idx].GetInt() };
			
			idx++;
		}
	}
	if (!document.HasMember("Tiles"))return Field{ cells };

	// テスト用にタイル情報がある場合読み込んで入力する
	auto tiles = document["Tiles"].GetArray();
	for (size_t i : step(size.y))
	{
		for (size_t k : step(size.x))
		{
			switch (tiles[i].GetString()[k])
			{
			case 'a':
				cells[{k, i}].PaintedBy(TeamType::Blue);
				break;
			case 'b':
				cells[{k, i}].PaintedBy(TeamType::Red);
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
	for (size_t i : step(data_size.y)) {
		for (size_t k : step(data_size.x)) {
			cells[{k, i}] = { (rand() >> 7) % 10 != 0 ?
				(rand() >> 7) % 17 :
				-((rand() >> 7) % 17)
			};
			// データをコピー
			cells[{ size.x - 1 - k,size.y - 1 - i}] = cells[{k, i}];
			cells[{ k,size.y - 1 - i}] = cells[{k, i}];
			cells[{size.x - 1 - k,i}] = cells[{k, i}];
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
	for (size_t x = 0; x < size.x; x++)
	{
		for (size_t y = 0; y < size.y; y++)
		{
			if (this->cells[{x,y}].tile != other.cells[{x,y}].tile)
			{
				return false;
			}
		}
	}

	return true;
}
