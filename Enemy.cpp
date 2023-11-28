#include "Enemy.h"

//Private Functions



void Enemy::initVariables()
{	
	pointCount = rand() % 50 + 20; //min = 3 max = 10
	type = 0;
	speed = static_cast<float>(pointCount * 0.2);
	hpMax = static_cast<int>(pointCount);
	hp = hpMax;
	damage = pointCount / 4;
	points = pointCount;
}

void Enemy::initShape()
{
	shape.setRadius(pointCount * 1.2);
	shape.setPointCount(pointCount);
	shape.setFillColor(
		sf::Color(
			rand() % 255 + 1,
			rand() % 255 + 1,
			rand() % 255
		));
}

//Con/Des
Enemy::Enemy(float pos_x, float pos_y)
{	
	initVariables();
	initShape();
	shape.setPosition(pos_x, pos_y);
}

Enemy::~Enemy()
{

}

//Accesors
const sf::FloatRect Enemy::getBounds() const
{
	return shape.getGlobalBounds();
}

const int& Enemy::getPoints() const
{
	return  points;
}

const int& Enemy::getDamage() const
{
	return damage;
}




//Functions 
void Enemy::update()
{
	shape.move(0.f, speed);
}

void Enemy::render(sf::RenderTarget* target)
{
	target->draw(shape);
}
