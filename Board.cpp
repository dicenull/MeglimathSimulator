#include"Board.h"

#define BOOST_PYTHON_STATIC_LIB    
#include<boost\python\tuple.hpp>

void Board::init_board(int turn, int start_player, int width, int height) {
	gamelogic.reset(new GameLogic(turn, { static_cast< size_t >( height ),static_cast< size_t >( width ) }));
	first = !start_player ? TeamType::A : TeamType::B;
	this->turn = first;
	gamelogic->initAgentsPos();
}
void Board::do_move(int move) {
	auto team = gamelogic->getTeamLogics()[0];
	int move1 = move / 17;
	int move2 = move % 17;
	Action act1{ move1 / 8 };
	Action act2{ move2 / 8 };
	Direction dir1{ ( int )act1 == 8 ? 8 : move1 % 8 };
	Direction dir2{ ( int )act1 == 8 ? 8 : move2 % 8 };
	Think think{ {act1,dir1},{act2,dir2} };
	_thinks[turn] = think;
	if ( turn != first ) {
		gamelogic->NextTurn(_thinks);
	}
	turn = turn == TeamType::A ? TeamType::B : TeamType::A;
}
py::tuple Board::has_a_winner()const {
	return py::make_tuple(gamelogic->GetWinner() != -1, gamelogic->GetWinner());
}
py::tuple Board::game_end()const {
	return py::make_tuple(gamelogic->GetGameEnd(), gamelogic->GetWinner());
}
int Board::get_current_player()const {
	return turn == TeamType::A ? 0 : 1;
}
np::ndarray Board::get_current_state()const {
	auto cells = gamelogic->GetField().GetCells();
	py::tuple shape = py::make_tuple(cells.width(), cells.height());
	np::ndarray ret = np::zeros(shape, np::dtype::get_builtin<long long>());
	for ( int x = 0; x < cells.width(); x++ ) {
		for ( int y = 0; y < cells.height(); y++ ) {
			switch ( cells[y][x].GetTile()) {
			case TileType::A:
				ret[x][y] = 0; break;
			case TileType::B:
				ret[x][y] = 1; break;
			default:
				ret[x][y] = -1;
				break;
			}
		}
	}
	return ret;
}
np::ndarray Board::get_board_state()const {
	auto cells = gamelogic->GetField().GetCells();
	py::tuple shape = py::make_tuple(cells.width(), cells.height());
	np::ndarray ret = np::zeros(shape, np::dtype::get_builtin<long long>());
	for ( int x = 0; x < cells.width(); x++ ) {
		for ( int y = 0; y < cells.height(); y++ ) {
			ret[x][y] = cells[y][x].GetPoint();
		}
	}
	return ret;
}
np::ndarray Board::get_player_state()const {
	auto cells = gamelogic->GetField().GetCells();
	py::tuple shape = py::make_tuple(cells.width(), cells.height());
	np::ndarray ret = np::zeros(shape, np::dtype::get_builtin<long long>());
	auto ag = gamelogic->GetAgents();
	for ( int i = 0; i < ag.size(); i++ ) {
		auto p = ag[i].GetPosition();
		ret[p.x][p.y] = i + 1;
	}
	return ret;
}
int Board::get_remain_turn() const {
	return this->gamelogic->GetTurn();
}
int Board::get_point(int player) const {
	auto pl = !player ? TeamType::A : TeamType::B;
	return this->gamelogic->GetField().GetTotalPoints()[static_cast< int >( pl )];
}
py::list Board::get_availables()const {
	py::list ret{};
	auto team = gamelogic->getTeamLogics()[0];
	for ( int move = 0; move < 17 * 17; move++ ) {
		int move1 = move / 17;
		int move2 = move % 17;
		Action act1{ move1 / 8 };
		Action act2{ move2 / 8 };
		Direction dir1{ ( int )act1 == 8 ? 8 : move1 % 8 };
		Direction dir2{ ( int )act1 == 8 ? 8 : move2 % 8 };
		Think think{ { act1,dir1 },{ act2,dir2 } };

		if ( gamelogic->IsThinkAble(turn, think) )ret.append(move);
	}
	return ret;
}
Board::Board() :gamelogic(new GameLogic(60)) {
	gamelogic->initAgentsPos();
}

Board::Board(const Board &) : gamelogic(new GameLogic(60)) {
	gamelogic->initAgentsPos();
}
