#pragma once

#include "GameRender.hpp"

class GameController
{
	GameModel *m_game;
	GameRender *m_render;
	Registry reg;
public:
	GameController(GameModel *game, GameRender *render);
	~GameController();
	void Run();
};
