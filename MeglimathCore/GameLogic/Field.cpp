#include "Field.h"
#include"util.h"
int Field::aggregateAreaPoint(TileType tile)
{
	_status = _Grid<bool>(_cells.size() + _Point<int>(2, 2));

	dfsAreaPoint(_Point<int>(0, 0), tile);

	int area_point = 0;
	for (size_t i : step(_status.width()))
	{
		for (size_t k : step(_status.height()))
		{
			if (_status[k][i])
			{
				continue;
			}

			auto cell = _cells[k - 1][i - 1];
			if (cell.GetTile() == tile)
			{
				continue;
			}
			
			area_point += std::abs(cell.GetPoint());
		}
	}

	return area_point;
}


void Field::dfsAreaPoint(_Point<> pos, TileType tile)
{
	// 範囲外なら終了
	if (pos.x < 0 || pos.x > _cells.width() + 1
		|| pos.y < 0 || pos.y > _cells.height() + 1)
	{
		return;
	}

	// 探索済みなら終了
	if (_status[pos.y][pos.x] == true)
	{
		return;
	}

	_status[pos.y][pos.x] = true;
	if (pos.x == 0 || pos.x == _cells.width() + 1 ||
		pos.y == 0 || pos.y == _cells.height() + 1)
	{
		// 端は探索のみ行う
	}
	else if (_cells[pos.y - 1][pos.x - 1].GetTile() == tile)
	{
		// 調査中のタイルが置かれていたら終了
		return;
	}

	_Point<> delta[] = { _Point<int>(0, 1), _Point<int>(1, 0), _Point<int>(0, -1), _Point<int>(-1, 0) };

	// 四方へ探索する
	for (int i : step(4))
	{
		dfsAreaPoint(pos + delta[i], tile);
	}
}

int Field::aggregateTilePoint(TileType tile)
{
	int sum_tile_point = 0;

	//	タイルの種類が一致するセルの得点の合計を計算する
	for (size_t i : step(_cells.width()))
	{
		for (size_t k : step(_cells.height()))
		{
			if (_cells[k][i].GetTile() == tile)
			{
				sum_tile_point += _cells[k][i].GetPoint();
			}
		}
	}

	return sum_tile_point;
}

int Field::aggregateTotalPoint(TileType tile)
{
	return aggregateAreaPoint(tile) + aggregateTilePoint(tile);
}

void Field::UpdatePoint()
{
	for (int i : step(2))
	{
		_points[i] =
		{ aggregateAreaPoint((TileType)i), aggregateTilePoint((TileType)i) };
	}
}

_Grid<Cell> Field::GetCells() const
{
	return _cells;
}

std::vector<int> Field::GetAreaPoints() const
{
	return std::vector<int>{_points[0].GetArea(), _points[1].GetArea() };
}

std::vector<int> Field::GetTilePoints() const
{
	return std::vector<int>{ _points[0].GetTile(), _points[1].GetTile() };
}

std::vector<int> Field::GetTotalPoints() const
{
	return std::vector<int>{ _points[0].GetTotal(), _points[1].GetTotal() };
}

void Field::PaintCell(_Point<> pos, TeamType team)
{
	_cells[pos.y][pos.x].PaintedBy(team);
}

void Field::RemoveTile(_Point<> pos)
{
	_cells[pos.y][pos.x].RemoveTile();
}

bool Field::IsInField(_Point<> pos) const
{
	return (0 <= pos.x && pos.x < _cells.width()) && (0 <= pos.y && pos.y < _cells.height());
}

Step Field::DecideStepByDirection(_Point<> pos, Direction dir) const
{
	if (dir == Direction::Stop)
	{
		return Step{ Action::Stop, Direction::Stop };
	}

	// 座標から指定の方向に進んだ後の座標
	_Point<int> next_pos = pos+Transform::DirToDelta(dir);

	if (!IsInField(next_pos))
	{
		return Step{ Action::Stop, Direction::Stop };
	}

	// 進んだ先のタイルの有無でアクションを決める
	if (_cells[next_pos.y][next_pos.x].GetTile() == TileType::None)
	{
		return Step{ Action::Move, dir };
	}
	else
	{
		return Step{ Action::RemoveTile, dir };
	}
}

Field::Field():Field(_Point<size_t>{6,6})
{
}

void Field::operator=(const Field & other)
{
	_cells = other._cells;
}


Field::Field(_Size size)
{
	// 入力されるタイルポイントの数
	_Size data_size = _Size((size.x + 1) / 2, (size.y + 1) / 2);
	_cells = _Grid<Cell>(size);
	for (int i = 0; i < _cells.width(); i++) {
		for (int j = 0; j < _cells.height(); j++) {
			_cells[i][j] = (rand() >> 7) % 33 - 16;
			_cells[size.y - 1 - i][size.x - 1 - j] = _cells[i][j];
			_cells[size.y - 1 - i][j] = _cells[i][j];
			_cells[i][size.x - 1 - j] = _cells[i][j];
		}
	}

}

Field::Field(_Grid<Cell> cells) :_cells(cells)
{
	//_cells = cells;
}

Field::Field(std::string json)
{
	rapidjson::Document document;
	document.Parse(json.data());
	_Size size = _Size{ document["Size"].GetString() };
	auto points = document["Points"].GetArray();

	// 入力されるタイルポイントの数
	_Size data_size = _Size((size.x + 1) / 2, (size.y + 1) / 2);

	_cells = _Grid<Cell>(size);

	// タイルポイントをグリッド状に成型して入力
	int idx = 0;
	for (int i : step(data_size.y))
	{
		for (int k : step(data_size.x))
		{
			_cells[i][k] = points[idx].GetInt();
			// データをコピー
			_cells[size.y - 1 - i][size.x - 1 - k] = _cells[i][k];
			_cells[size.y - 1 - i][k] = _cells[i][k];
			_cells[i][size.x - 1 - k] = _cells[i][k];

			idx++;
		}
	}
	if (!document.HasMember("Tiles"))return;

	// テスト用にタイル情報がある場合読み込んで入力する
	auto tiles = document["Tiles"].GetArray();
	for (int i : step(size.y))
	{
		for (int k : step(size.x))
		{
			switch (tiles[i].GetString()[k])
			{
			case 'a':
				_cells[i][k].PaintedBy(TeamType::A);
				break;
			case 'b':
				_cells[i][k].PaintedBy(TeamType::B);
				break;
			default:
				break;
			}
		}
	}

}


Field::~Field()
{
}
