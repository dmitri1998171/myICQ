#include "GameController.hpp"

GameController::GameController(GameModel *game, GameRender *render)
{
	m_game = game;
	m_render = render;
}

GameController::~GameController() {}

void GameController::Run()
{
	sf::Event event;
	while (m_render->window().isOpen())
	{
		while (m_render->window().pollEvent(event))
		{
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                m_render->window().close();
                exit(0);
            }

			/* Click on UI elements */ 
        
            if (event.type == Event::MouseButtonReleased) 
                if (event.mouseButton.button == Mouse::Left) m_game->onClick(&m_render->window());

			// input_rect is active
            if(reg.write_flag) {   
                if (event.type == Event::TextEntered) m_game->textInput(&event);

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Enter) m_game->KeyPressedEnter();
                    if (event.key.code == Keyboard::BackSpace ) m_game->KeyPressedBackspace();
                }
            }
       
		}
		m_render->Render();
	}
}
