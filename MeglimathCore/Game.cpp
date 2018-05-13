#include "Game.h"

GameInfo Game::GetGameInfo() const
{
	return GameInfo(_gamelogic.GetField(), _gamelogic.GetTurn(), LogicUtil::toS3dHashTable(_gamelogic.GetAgentMap()));
}

HashTable<TeamType, Array<Agent>> Game::GetAgentMap() const
{
	return LogicUtil::toS3dHashTable(_gamelogic.GetAgentMap());
}

Array<Agent> Game::GetAgents() const
{
	return LogicUtil::toS3dArray(_gamelogic.GetAgents());
}

void Game::initAgentsPos()
{
	Size size = LogicUtil::toS3dPoint(_gamelogic.GetField().GetCells().size());

	initAgentsPos(Point(Random((size.x - 2) / 2), Random((size.y - 2) / 2)));
}

void Game::initAgentsPos(Point init_pos)
{
	_gamelogic.initAgentsPos({ init_pos.x,init_pos.y });
}

void Game::setTeam(std::shared_ptr<Team> team_a, std::shared_ptr<Team> team_b)
{
	_teams.append({ team_a, team_b });
}

Game::Game(const String path)
{
	TextReader reader{ path };
	_gamelogic.InitalizeFromJson(reader.readAll().narrow());
}

bool Game::IsReady()
{
	return _teams[0]->IsReady() && _teams[1]->IsReady();
}

int Game::GetTurn() const
{
	return _gamelogic.GetTurn();
}
void Game::NextTurn()
{
	if (_gamelogic.GetTurn() <= 0)
	{
		return;
	}
	
	GameInfo info = GetGameInfo();
	auto agents_map = GetAgentMap();
	auto agents = GetAgents();

	_thinks[TeamType::A] = Think(_teams[0]->NextThink(info));
	_thinks[TeamType::B] = Think(_teams[1]->NextThink(info));

	_gamelogic.NextTurn(LogicUtil::fromS3dHashTable(_thinks));

}

void Game::Update()
{

}

Field Game::GetField() const
{
	return _gamelogic.GetField();
}

HashTable<TeamType, Think> Game::GetThinks() const
{
	return _thinks;
}

Game::Game()
{

}

Game::~Game()
{
}
