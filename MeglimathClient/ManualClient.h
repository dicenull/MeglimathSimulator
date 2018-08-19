#pragma once
#include "Client.h"

#include <Siv3D.hpp>

class ManualClient :
	public Client
{
protected:
	virtual Optional<Step> decideStep() = 0;

private:
	Optional<Step> _steps[2];

public:
	void Update(GameInfo info) override
	{
		if (IsReady())
		{
			return;
		}

		auto & decide_step
			= (_steps[0].has_value() ? _steps[1] : _steps[0]);

		decide_step = decideStep();

		if (_steps[0].has_value() && _steps[1].has_value())
		{
			_think = { _steps[0].value(), _steps[1].value() };

			_steps[0].reset();
			_steps[1].reset();
			_is_ready = true;
		}
	}

	void Initialize() override
	{
		_is_ready = false;
	}

public:
	void SetTeamType(TeamType type);

	Array<Optional<Step>> GetSteps();

public:
	ManualClient();
	ManualClient(TeamType type);
	~ManualClient();
};

