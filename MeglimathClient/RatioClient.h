#pragma once
#include "Client.h"

//TeamToTile使うためのインクルード
#include "T_Monte_Carlo.h"

namespace RC {
	int movedByway(int way);
}

class RatioClient :
	public Client {
private:
	int lastMovedBy[2] = { 8,8 };//現在位置にはどのマスから来たのかを格納しておく。
								 //例えば、前のターン0のの方向に進むことによって現在のマスに来たのなら、4が格納される
	int max[2] = { 8,8};//周辺のマスの中で一番ポイントが高いマス
public:
	String Name() override {
		return U"RatioClient";
	}

	void Initialize() override {
		_is_ready = false;
	}

	//周辺のマスの内、一番得点が大きいマスに行く
	//周辺のマスの内、相手のマスが一番得点の大きいマスであれば、それを除去する。
	void Update(GameInfo info) override {

		//if (IsReady()) {
		//	return;
		//}

		Field tem = info.GetField();

		Array<Agent> agents = info.GetAgents(_type);

		_Point<> preP[2];

		//方向決めるためのポイント入れとくやつ
		int temppoint = -3;
		bool didnewway = false;//新しく次の手が格納されたか

		for (int i = 0; i < 2; i++) {
			preP[i] = agents[i].position;
			for (int j = 0; j < 8; j++) {
				if (tem.IsInField(preP[i] + Transform::DirToDelta(Direction(j)))) {//選んだマスがフィールド内か
					if (tem.cells[(preP[i] + Transform::DirToDelta(Direction(j))).y][(preP[i] + Transform::DirToDelta(Direction(j))).x].point > temppoint//現在保存している最大ポイントより参照しているポイントの方が高いかどうか
						|| (tem.cells[(preP[i] + Transform::DirToDelta(Direction(j))).y][(preP[i] + Transform::DirToDelta(Direction(j))).x].point == temppoint && Random(0,100) % 2 == 0)) {//同じだったらランダム
						if ((lastMovedBy[i] == 8 || j != lastMovedBy[i]) && tem.cells[(preP[i] + Transform::DirToDelta(Direction(j))).y][(preP[i] + Transform::DirToDelta(Direction(j))).x].tile != TMC::TeamtoTile(_type)) {//最後に来た方向にもう一度行かないように、自分のマスは除外
							if (i == 0 || (i == 1 && !(agents[0].position + Transform::DirToDelta(Direction(max[0])) == preP[i] + Transform::DirToDelta(Direction(j))))) {//エージェント同士が拮抗しないように、一人目のエージェントが進もうとしているマスには二人目は行かないようにする
								max[i] = j;
								temppoint = tem.cells[(preP[i] + Transform::DirToDelta(Direction(j))).y][(preP[i] + Transform::DirToDelta(Direction(j))).x].point;
								didnewway = true;
							}
	
						}
					}
				}
			}
			if (didnewway == false) {//最適なマスが見つからなかったら、一つ前のマスに戻る
				max[i] = lastMovedBy[i];
			}
			temppoint = 0;
			lastMovedBy[i] = RC::movedByway(max[i]);
			didnewway = false;
		}

		_think = Think{ tem.DecideStepByDirection(preP[0], Direction(max[0])), tem.DecideStepByDirection(preP[1], Direction(max[1])) };

		_is_ready = true;
	}



public:
	RatioClient(TeamType type);
	~RatioClient();
};