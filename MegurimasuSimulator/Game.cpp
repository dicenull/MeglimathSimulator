#include "Game.h"


void Game::Update()
{

}

void Game::Draw()
{
	_field.Draw();
}

Game::Game(const Field &field, Team team_a, Team team_b)
{
	_field = field;
	_teams[0] = team_a;
	_teams[1] = team_b;
}


Game::~Game()
{
}
