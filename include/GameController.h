#pragma once
#include "GameRender.h"

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
