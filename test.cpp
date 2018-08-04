#include"Board.h"
#include<memory>
int main() {
	std::unique_ptr<Board> B(new Board());
	B->init_board(0);
	B->has_a_winner();
	//B->get_availables();
}