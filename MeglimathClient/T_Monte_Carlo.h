#pragma once
#include <Siv3D.hpp>
#include "Client.h"
#include "../MeglimathCore/GameLogic/Think.h"

class T_Monte_Carlo : public Client {
private:

	/// <summary>
	/// ������̌���T���֐�
	/// </summary>
	/// <param name="movelist">����܂œ������}�X</param>
	/// <param name="agent">�����߂�Ώۂ̃G�[�W�F���g</param>
	/// <param name="field">�t�B�[���h���</param>
	/// <returns>������</returns>
	int decideMove(Array<int> *movelist, Agent agent, Field field);

	/// <summary>
	/// �N�C�b�N�\�[�g
	/// </summary>
	/// <param name="target">�\�[�g�Ώ�</param>
	/// <param name="left">�\�[�g�Ώۂ̔z��̈�ԍ��̗v�f</param>
	/// <param name="right">�\�[�g�Ώۂ̈�ԉE�̗v�f</param>
	void sort(Array <std::pair<Array<int>, int> > *target, int left, int right);

	/// <summary>
	/// TeamType��TileType�ɕϊ�����֐�
	/// </summary>
	TileType TeamtoTile(TeamType t);

public:
	String Name() override
	{
		return U"T MonteCarlo";
	}

	void Update(GameInfo info) override {

		if (IsReady())
		{
			return;
		}

		//�G�[�W�F���g1�A2�̃��[�g�Ƃ��̃��[�g�ɂ���ē�����^�C���|�C���g��Z�b�g�ŕ�������Ă����ϐ�
		Array< std::pair<Array<int>, int> > agent1;
		Array< std::pair<Array<int>, int> > agent2;


		std::pair<Array<int>, int> agenttemp1;

 		agenttemp1.second = 0;

		Field tem = info.GetField();
		Array<Agent> agents = info.GetAgents(_type);
		int temppoint;
		Point preP;

		while (1) {
			//�G�[�W�F���g�̈ړ������_����20��v�Z�A���ʂ�z���push
			for (int i = 0; i < 20; i++) {
				temppoint = decideMove(&(agenttemp1.first), agents[0], tem);
				agenttemp1.second += temppoint;
			}
			agent1.push_back(agenttemp1);
			agenttemp1.first.clear();
			agenttemp1.second = 0;


			//�v�Z���ʂ�2�ʂ�ȏ�ɂȂ�����^�C���|�C���g���Ń\�[�g�J�n
			if (agent1.size() > 2) {
				sort(&agent1, 0, agent1.size() - 1);
				//�^�C���|�C���g����ԍ������[�g�ƁA2�Ԗڂɍ������[�g�̈�ԍŏ��̈ړ��悪������1�}�X�����Y���Ă�����A��ԃ^�C���|�C���g���������[�g�̍ŏ��̈ړ����ŏI�I�Ȉړ���Ƃ���
				if (agent1[agent1.size() - 1].first[0] == agent1[agent1.size() - 2].first[0] || agent1[agent1.size() - 1].first[0] == (agent1[agent1.size() - 2].first[0]) + 1 || agent1[agent1.size() - 1].first[0] ==( agent1[agent1.size() - 2].first[0])- 1
					|| agent1.size() > 100) {
					break;
				}
			}

		}

		while (1) {
			for (int i = 0; i < 20; i++) {
				temppoint = decideMove(&(agenttemp1.first), agents[1], tem);
				agenttemp1.second += temppoint;
			}

			agent2.push_back(agenttemp1);
			agenttemp1.first.clear();
			agenttemp1.second = 0;


			if (agent2.size() > 2) {
				sort(&agent2, 0, agent2.size() - 1);
				if (agent2[agent2.size() - 1].first[0] == agent2[agent2.size() - 2].first[0] || agent2[agent2.size() - 1].first[0] == (agent2[agent2.size() - 2].first[0]) + 1 || agent2[agent2.size() - 1].first[0] ==( agent2[agent2.size() - 2].first[0]) - 1
					|| agent2.size() > 100) {
					break;
				}
			}

		}

		_think = Think{ tem.DecideStepByDirection(agents[0].GetPosition(), Direction(agent1[agent1.size() - 1].first[0])), tem.DecideStepByDirection(agents[1].GetPosition(), Direction(agent2[agent2.size() - 1].first[0])) } ;
		_is_ready = true;
	}

	void Initialize() override
	{
		_is_ready = false;
	}

public:
	T_Monte_Carlo(TeamType type);
	~T_Monte_Carlo();
};