#pragma once
#include "Field.h"
#include "Team.h"


	class Game
	{
	private:
		Field _field;
		Team _teams[2];

	public:
		void Update();
		void Draw();

	public:
		Game(const Field &field, Team team_a, Team team_b);
		virtual ~Game();
	};
