#include "T_Monte_Carlo.h"

TileType T_Monte_Carlo::TeamtoTile(TeamType t) {
	switch (t) {
	case TeamType::A:
		return TileType::A;
	case TeamType::B:
		return TileType::B;
	}
}

int T_Monte_Carlo::decideMove(Array<int> *movelist, Agent agent, Field field) {

	int preMove;
	Point preP;
	if ((*movelist).isEmpty()) {
		while (1) {
			//周囲8マスをランダムに選ぶ
			preMove = Random(0, 7);
			preP = Point(agent.GetPosition().x + Transform::DirToDelta(Direction(preMove)).x, agent.GetPosition().y + Transform::DirToDelta(Direction(preMove)).y);
			//ランダムに選んだマスがフィールド内か確認する。フィールド外だったらやり直し
			//自チームのタイルは検索から除外するようにしたのがコメントアウトされている部分だが、これを適用するととんでもなく処理が長くなる
			if (field.IsInField(preP) /*&& field.GetCells()[preP.y][preP.x].GetTile() != TeamtoTile(this->_type)*/){
				(*movelist).push_back(preMove);
				return field.GetCells()[preP.y][preP.x].GetPoint();
			}
		}
	}
	else {
		//これまで動いたマスリストにある動作記録を反映させ、動作リストを適用するとどこにエージェントがいることになるのか計算する
		preP = Point(agent.GetPosition().x, agent.GetPosition().y);
		for (int i = 0; i < (*movelist).size(); i++) {
			preP.x += Transform::DirToDelta(Direction((*movelist)[i])).x;
			preP.y += Transform::DirToDelta(Direction((*movelist)[i])).y;
		}

		while (1) {
			//周囲8マスをランダムに選ぶ
			preMove = Random(0, 7);
			//選んだマスがフィールド内か確認する
			//コメントアウト部分は自チームのタイルを検索から除外するもの。適用すると処理が長くなる
			if (field.IsInField(Point(preP.x + Transform::DirToDelta(Direction(preMove)).x, preP.y + Transform::DirToDelta(Direction(preMove)).y))
				/*&& field.GetCells()[preP.y][preP.x].GetTile() != TeamtoTile(this->_type)*/) {
				(*movelist).push_back(preMove);
				return field.GetCells()[preP.y][preP.x].GetPoint();
			}
		}
	}
}

void T_Monte_Carlo::sort(Array<std::pair<Array<int>, int>> *target, int left, int right) {
	int i, j, pivot;
	std::pair<Array<int>, int> temp;

	i = left;
	j = right;

	pivot = (*target)[left].second;

	while (1) {
		while ((*target)[i].second < pivot) {

			i++;
		}
	
		while (pivot < (*target)[j].second) {

			j--;

		}

		if (i >= j) {
			break;
		}

		temp = (*target)[i];
		(*target)[i] = (*target)[j];
		(*target)[j] = temp;
		i++;
		j--;
	}

	if (left < i - 1) sort(target, left, i - 1);
	if (j + 1 < right) sort(target, j + 1, right);
}

T_Monte_Carlo::T_Monte_Carlo(TeamType type)
	:Team(type, Agent(), Agent()) {

}

T_Monte_Carlo::T_Monte_Carlo(TeamType type, Agent agent1, Agent agent2)
	: Team(type, agent1, agent2) {

}