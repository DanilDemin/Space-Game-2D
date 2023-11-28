#include "Game.h"

//Private functions
void Game::initWindow()
{
	window = new sf::RenderWindow(sf::VideoMode(1920, 1600),
		"Space Game", sf::Style::Close | sf::Style::Titlebar);
	window->setFramerateLimit(90);
	window->setVerticalSyncEnabled(false);

}

void Game::initTextures()
{
	textures["BULLET"] = new sf::Texture();
	textures["BULLET"]->loadFromFile("Textures/bullet.png");
}

void Game::initGUI()
{
	//LoadFont

	if (!font.loadFromFile("Fonts/Aller_BdIt.ttf"))
	{
		std::cout << "ERROR Game!" << '\n';

	}

	//init point text
	pointText.setFont(font);
	pointText.setCharacterSize(60);
	pointText.setFillColor(sf::Color::White);
	pointText.setString("test");

	gameOverText.setFont(font);
	gameOverText.setCharacterSize(150);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setString("Game Over!");
	gameOverText.setPosition(
		window->getSize().x / 2.f - gameOverText.getGlobalBounds().width / 2.f,
		window->getSize().y / 2.f - gameOverText.getGlobalBounds().height / 2.f);
	

	//init player gui
	playerHpBar.setSize(sf::Vector2f(300.f, 30.f));
	playerHpBar.setFillColor(sf::Color::Red);
	playerHpBar.setPosition(sf::Vector2f(20.f, 100.f));

	/*playerHpBar.setPosition(player->getPos());*/


	playerHpBarBack = playerHpBar;
	playerHpBarBack.setFillColor(sf::Color(39, 32, 30));

}

void Game::initWorld()
{	

	if (!worldBackgroundTex.loadFromFile("Textures/Background.jpg"))
	{
		std::cout << "Game could not load background" << '\n';
	}
	worldBackground.setTexture(worldBackgroundTex);
	worldBackground.setScale(0.4f, 0.4f);

}

void Game::initSysytems()
{
	points = 0;
}

void Game::initPlayer()
{
	player = new Player();
}

void Game::initEnemies()
{	
	spawnTimerMax = 50.f;
	spawnTimer = spawnTimerMax;
}

//Con/Des
Game::Game()
{
	initWindow();
	initTextures();
	initWorld();
	initSysytems();
	initGUI();
	initPlayer();
	initEnemies();
}

Game::~Game()
{
	delete window;
	delete player;

	//Delete textures
	for (auto &i : textures)
	{
		delete i.second;
	}

	//Delete bullets
	for (auto* i : bullets)
	{
		delete i;
	}

	//Delete enemies
	for (auto* i : enemies)
	{
		delete i;
	}

}

//Functions
void Game::run()
{	
	while (window->isOpen())
	{	
		updatePollEvents();
		if (player->getHp() > 0)
		{
			update();
		}
		
		render();
	}
}

void Game::updatePollEvents()
{	
	sf::Event e;
	while (window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed)
		{
			window->close();
		}
		if (e.Event::KeyPressed && e.Event::key.code ==
			sf::Keyboard::Escape)
		{
			window->close();
		}
	}
}

void Game::updateInput()
{
	//Move player
	// Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		player->move(-1.f, 0.f);
	}
	//Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		player->move(1.f, 0.f);
	}
	//Up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		player->move(0.f, -1.f);
	}
	//Buttom
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		player->move(0.f, 1.f);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && player->canAttack())
	{
		bullets.push_back(
			new Bullet(
			textures["BULLET"],
			player->getPos().x + player->getBounds().width / 2.f - 35,
			player->getPos().y,
			0.f,
			-1.f,
			20.f
		));
	}

	//CHECK WITOUT MOUSEl
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K) && player->canAttack())
	{
		bullets.push_back(
			new Bullet(
				textures["BULLET"],
				player->getPos().x + player->getBounds().width / 2.f - 35,
				player->getPos().y,
				0.f,
				-1.f,
				20.f
			));
	}
	 
}

void Game::updateGUI()
{
	std::stringstream ss;
	ss << "Points: " << points;
	pointText.setString(ss.str());

	//Update player GUI
	
	
	playerHpBar.setPosition(
		player->getPos().x + player->getBounds().width + 20.f,
		player->getPos().y);
	playerHpBarBack.setPosition(playerHpBar.getPosition());
	float hpPercent = static_cast<float>(player->getHp()) / player->getHpMax();
	playerHpBar.setSize(sf::Vector2f(300.f * hpPercent, playerHpBar.getSize().y));

}

void Game::updateWorld()
{

}

void Game::updateCollision()
{	
	//Left world collision
	if (player->getBounds().left < 0.f)
	{
		player->setPosition(0.f, player->getBounds().top);
	}
	//Right world collision
	else if (player->getBounds().left + player->getBounds().width 
		>= window->getSize().x)
	{
		player->setPosition(window->getSize().x - player->getBounds().width,
			player->getBounds().top);
	}

	//Top world collision
	if (player->getBounds().top < 0.f)
	{
		player->setPosition(player->getBounds().left, 0.f);
	}
	//Bottom world collision
	else if (player->getBounds().top + player->getBounds().height >= 
		window->getSize().y)
	{
		player->setPosition(player->getBounds().left,
			window->getSize().y - player->getBounds().height);
	}
}

void Game::updateBullets()
{	
	unsigned counter = 0;
	for (auto* bullet : bullets)
	{
		/*bullet->render(window);*/
		bullet->update();

		//bullet culling (top og the screen)
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{	
			//Delete bullet
			delete bullets.at(counter);
			bullets.erase(bullets.begin() + counter);
		}

		++counter;
	}


}

void Game::updateEnemies()
{	
	//Spawning
	spawnTimer += 2.0f;
	if (spawnTimer >= spawnTimerMax)
	{	
		enemies.push_back(
			new Enemy(
			rand() % window->getSize().x,
			-100.f
			));
		spawnTimer = 0.f;
	}
	
	//Update
	unsigned counter = 0;
	for (auto* enemy : enemies)
	{	
		enemy->update();

		//enemy culling (bottom of the screen)
		if (enemy->getBounds().top > window->getSize().y)
		{
			//Delete enemy
			delete enemies.at(counter);
			enemies.erase(enemies.begin() + counter);
			 
			/*std::cout << "DeLETE" << '\n';*/
		}
		//Enemy player collision
		else if (enemy->getBounds().intersects(player->getBounds()))
		{
			//Delete enemy
			player->loseHp(enemies.at(counter)->getDamage());
			delete enemies.at(counter);
			enemies.erase(enemies.begin() + counter);
			 
			
		}


		++counter;
	}

}

void Game::updateCombat()
{
	for (int i = 0; i < enemies.size(); ++i)
	{	
		bool enemy_deleted = false;

		for (size_t k = 0; k < bullets.size() && enemy_deleted == false; ++k)
		{
			if (enemies[i]->getBounds().intersects(bullets[k]->getBounds()))
			{	
				points += enemies[i]->getPoints();

				delete enemies[i];
				enemies.erase(enemies.begin() + i);

				delete bullets[k];
				bullets.erase(bullets.begin() + k);


				enemy_deleted = true;
			}
		}
	}
}

void Game::update()
{
	//Move Player
	updateInput();

	player->update();

	updateCollision();

	//Move bullets
	updateBullets();

	//Move enemies
	updateEnemies();

	updateCombat();

	updateGUI();

	updateWorld();

}

void Game::renderGUI()
{
	window->draw(pointText);
	window->draw(playerHpBarBack);
	window->draw(playerHpBar);

}

void Game::renderWorld()
{
	window->draw(worldBackground);
}

void Game::render()
{
	window->clear();

	//Draw world
	renderWorld();

	//Draw all the stuffs
	player->render(*window);

	for (auto* bullet : bullets)
	{
		bullet->render(window);
	}

	for (auto* enemy : enemies)
	{
		enemy->render(window);
	}

	renderGUI();

	//Game over screen
	if (player->getHp() <= 0)
	{
		window->draw(gameOverText);
	}
	
	window->display();
}
