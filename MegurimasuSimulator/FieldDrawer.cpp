#include "FieldDrawer.h"


void FieldDrawer::SetOrigin(Point origin) 
{
	_origin = origin;
}

Point FieldDrawer::GetOrigin() const
{
	return _origin;
}

void FieldDrawer::Draw(const GameInfo & info) const
{

	// セルとタイルポイントの描画
	Rect r;
	auto cells = info.GetField().GetCells();
	Array<Agent> a_agents = info.GetAgents(TeamType::A);
	Array<Agent> b_agents = info.GetAgents(TeamType::B);
	
	for (int i = 0; i < cells.size().x; i++)
	{
		for (int k = 0; k < cells.size().y; k++)
		{
			Point pos(i, k);
			r = Rect(_origin + pos * _c_size, _c_size);

			r.draw(Transform::ColorOf(cells[k][i].GetTile()));

			// エージェントがいる場合枠の色を変える
			Color frame_color = Palette::Gray;
			auto pos_lamda = [pos](const Agent & agent) {return agent.GetPosition() == pos; };

			if (a_agents.includes_if(pos_lamda))
			{
				frame_color = Palette::Magenta;
			}
			else if (b_agents.includes_if(pos_lamda))
			{
				frame_color = Palette::Cyan;
			}

			r.drawFrame(2.0, 0, frame_color);

			FontAsset(U"Cell")(cells[k][i].GetPoint()).drawAt(r.center(), Palette::Black);
		}
	}


}

FieldDrawer::FieldDrawer()
{
}


FieldDrawer::~FieldDrawer()
{
}

void FieldDrawer::operator=(const FieldDrawer & other)
{
	_origin = other._origin;
}
