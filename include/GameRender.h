#pragma once
#include "GameModel.h"

class GameRender
{
	GameModel *m_game;
	sf::RenderWindow m_window;
	sf::Text m_text;

	Registry reg;
	IDrawUI drawUI;
	IGUI ui;
	
public:
	GameRender(GameModel *game);
	~GameRender();
	sf::RenderWindow& window() { return m_window; };
	bool Init();
	void Render();
	void initUI();
};
