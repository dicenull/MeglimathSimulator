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
	Think NextThink(GameInfo info) override
	{
		// s“®Œˆ’è‚µ‚Ä‚¢‚È‚¢ê‡‚Ìˆ—
		for (auto i : step(2))
		{
			if (!_steps[i].has_value())
			{
				_steps[i] = Step((Action)Random(0, 1), (Direction)Random(0, 7));
			}
		}

		Think next_think = { _steps[0].value(), _steps[1].value() };

		// ‰Šú‰»ˆ—
		for (auto & _step : _steps)
		{
			_step.reset();
		}
		_is_ready = false;

		return next_think;
	}

	void Update() override
	{
		if (_is_ready)
		{
			return;
		}

		auto & decide_step
			= (_steps[0].has_value() ? _steps[1] : _steps[0]);

		decide_step = decideStep();

		if (_steps[0].has_value() && _steps[1].has_value())
		{
			_is_ready = true;
		}
	}

public:
	void SetTeamType(TeamType type);

	Array<Optional<Step>> GetSteps();

public:
	ManualClient();
	ManualClient(TeamType type);
	~ManualClient();
};

