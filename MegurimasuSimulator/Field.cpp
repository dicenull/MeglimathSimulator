#include "Field.h"

int Field::aggregateAreaPoint(TileType tile)
{
	_status = Grid<bool>(_cells.size() + Point(2, 2), true);

	dfsAreaPoint(Point(0, 0), tile);

	int area_point = 0;
	for (int i : step(_status.width()))
	{
		for (int k : step(_status.height()))
		{
			if (_status[k][i])
			{
				area_point += Abs(_cells[k - 1][i - 1].GetPoint());
			}
		}
	}

	return area_point;
}

void Field::initVariable()
{
	// 2チーム分の得点を初期化する	
	_total_point.append({ 0, 0 });
}

void Field::dfsAreaPoint(Point pos, TileType tile)
{
	// 範囲外なら終了
	if (pos.x < 0 || pos.x > _cells.width() + 1
		|| pos.y < 0 || pos.y > _cells.height() + 1)
	{
		return;
	}

	// 探索済みなら終了
	if (_status[pos.y][pos.x] == false)
	{
		return;
	}

	_status[pos.y][pos.x] = false;
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

	Point delta[] = { Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0) };

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
	for (int i : step(_cells.width()))
	{
		for (int k : step(_cells.height()))
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
	_total_point[0] = aggregateTotalPoint(TileType::A);
	_total_point[1] = aggregateTotalPoint(TileType::B);
}

Grid<Cell> Field::GetCells() const
{
	return _cells;
}

Array<int> Field::GetTotalPoint() const
{
	return _total_point;
}

void Field::PaintCell(Point pos, TeamType team)
{
	_cells[pos.y][pos.x].PaintedBy(team);
}

void Field::RemoveTile(Point pos)
{
	_cells[pos.y][pos.x].RemoveTile();
}

bool Field::IsInField(Point pos)
{
	return (0 <= pos.x && pos.x < _cells.width()) && (0 <= pos.y && pos.y < _cells.height());
}

Field::Field()
{
}

void Field::operator=(const Field & other)
{
	_cells = other._cells;
}


Field::Field(Size size)
	:Field(Grid<Cell>(size))
{}

Field::Field(Grid<Cell> cells)
{
	initVariable();
	_cells = cells;
}

Field::Field(FilePath file)
{
	initVariable();

	JSONReader json(file);

	Size size = json[U"Size"].get<Size>();
	auto points = json[U"Points"].arrayView();

	// 入力されるタイルポイントの数
	Size data_size = Size((size.x + 1) / 2, (size.y + 1) / 2);

	_cells = Grid<Cell>(size);

	// タイルポイントをグリッド状に成型して入力
	int idx = 0;
	for (int i : step(data_size.y))
	{
		for (int k : step(data_size.x))
		{
			_cells[i][k] = points[idx].get<int>();
			// データをコピー
			_cells[size.y - 1 - i][size.x - 1 - k] = _cells[i][k];
			_cells[size.y - 1 - i][k] = _cells[i][k];
			_cells[i][size.x - 1 - k] = _cells[i][k];

			idx++;
		}
	}

	if (json[U"Tiles"].isEmpty())
	{
		return;
	}

	// テスト用にタイル情報がある場合読み込んで入力する
	auto tiles = json[U"Tiles"].arrayView();

	for (int i : step(size.y))
	{
		for (int k : step(size.x))
		{
			switch (tiles[i].getString()[k])
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
