#include "Field.h"

Field Field::Create(FilePath file)
{
	JSONReader json(file);

	Size size = json[U"Size"].get<Size>();
	auto points = json[U"Points"].arrayView();

	// 入力されるタイルポイントの数
	Size data_size = Size((size.x + 1) / 2, (size.y + 1) / 2);

	Grid<Cell> cells = Grid<Cell>(size);

	// タイルポイントをグリッド状に成型して入力
	int idx = 0;
	for (int i : step(data_size.y))
	{
		for (int k : step(data_size.x))
		{
			cells[k][i] = points[idx].get<int>();
			// データをコピー
			cells[size.y - 1 - k][size.x - 1 - i] = cells[k][i];

			idx++;
		}
	}

	if (json[U"Tiles"].isEmpty())
	{
		return Field(cells);
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
				cells[k][i].PaintedBy(TeamType::A);
				break;
			case 'b':
				cells[k][i].PaintedBy(TeamType::B);
				break;
			default:
				break;
			}
		}
	}

	return Field(cells);
}

Grid<Cell> Field::GetCells() const
{
	return _cells;
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
{
	_cells = Grid<Cell>(size);
}

Field::Field(Grid<Cell> cells)
{
	_cells = cells;
}

Field::~Field()
{
}
