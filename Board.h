#pragma once
#include<memory>
#include"simulater/MeglimathCore/GameLogic/GameLogic.h"
#define BOOST_PYTHON_STATIC_LIB    
#define BOOST_NUMPY_STATIC_LIB    
#include<boost/python/numpy.hpp>
#include<boost/python.hpp>
namespace py = boost::python;
namespace np = boost::python::numpy;
class Board {
	std::unique_ptr<GameLogic> gamelogic;
	std::unordered_map<TeamType, Think> _thinks;
	TeamType turn{ TeamType::A };
	TeamType first{ TeamType::A };
public:
	void init_board(int turn = 60, int start_player = 0, int width = 6, int height = 6);
	void do_move(int move);
	py::tuple has_a_winner()const;
	py::tuple game_end()const;
	int get_current_player()const;
	np::ndarray get_current_state()const;
	np::ndarray get_board_state()const;
	np::ndarray get_player_state()const;
	int get_remain_turn()const;
	int get_point(int player = 0)const;
	py::list get_availables()const;
	Board();
	Board(const Board &);
};