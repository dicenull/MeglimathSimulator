#include"Board.h"
#include"simulater/MeglimathCore/GameLogic/GameLogic.h"
#define BOOST_PYTHON_STATIC_LIB  
#include<boost\python.hpp>
#include "deepcopy.h"
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(init_board_,init_board,0,4)

BOOST_PYTHON_MODULE(MeglimathPy) {
	using namespace boost::python;
	namespace np = boost::python::numpy;
	Py_Initialize();
	np::initialize();
	class_<Board>("Board")
		.def("init_board", &Board::init_board,init_board_())
		.def("do_move", &Board::do_move)
		.def("has_a_winner", &Board::has_a_winner)
		.def("game_end", &Board::game_end)
		.def("get_current_player", &Board::get_current_player)
		.def("get_current_state", &Board::get_current_state)
		.def("get_board_state", &Board::get_board_state)
		.def("get_player_state", &Board::get_player_state)
		.def("__deepcopy__",&generic__deepcopy__<Board>)
		.add_property("availables",&Board::get_availables)
		.add_property("remain_turn",&Board::get_remain_turn)
		.def("get_point",&Board::get_point)
		;
}