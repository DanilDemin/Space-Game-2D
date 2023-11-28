#pragma once

#include<SFML/Graphics.hpp>

class Enemy
{
private:
	unsigned pointCount;
	sf::CircleShape shape;
	int type;
	int hp;
	float speed;
	int hpMax;
	int damage;
	int points;
	
	//Private Functions
	void initVariables();
	void initShape();


public:
	//Con/Des
	Enemy(float pos_x, float pos_y);
	virtual ~Enemy();

	//Accesors
	const sf::FloatRect getBounds() const;
	const int& getPoints() const;
	const int& getDamage() const;


	//Functions 
	void update();
	void render(sf::RenderTarget* target);

};

