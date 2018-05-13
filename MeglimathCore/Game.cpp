#include "Game.h"

GameInfo Game::GetGameInfo() const
{
	HashTable<TeamType, Array<Agent>> agentmap;
	for (auto k : _gamelogic.GetAgentMap())agentmap[k.first] = k.second;
	return GameInfo(_gamelogic.GetField(), _gamelogic.GetTurn(), agentmap);
}

HashTable<TeamType, Array<Agent>> Game::GetAgentMap() const
{
	HashTable<TeamType, Array<Agent>> agentmap;
	for (auto k : _gamelogic.GetAgentMap())agentmap[k.first] = k.second;
	return agentmap;
}

Array<Agent> Game::GetAgents() const
{
	Array<Agent> agents;
	for (auto a : _gamelogic.GetAgents())agents.push_back(a);
	return agents;
}

void Game::initAgentsPos()
{
	Size size = Point(_gamelogic.GetField().GetCells().size().x, _gamelogic.GetField().GetCells().size().y);

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
	_gamelogic.InitalizeFromJson(path.toUTF32);
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
	
	std::unordered_map<TeamType,Think> th{};
	for (auto t = _thinks.cbegin(); t != _thinks.cend(); t++)th[t.key()] = t.value();
	_gamelogic.NextTurn(th);

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
