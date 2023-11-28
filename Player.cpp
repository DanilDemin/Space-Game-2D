#include "Player.h"

void Player::initVariables()
{
	movementSpeed = 20.f;
	attackCooldownMax = 4.f;
	attackCooldown = attackCooldownMax;
	
	hpMax = 100;
	hp = hpMax;
}

void Player::initTexture()
{
	if (!texture.loadFromFile("Textures/ship.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXUTE::Could not load the filw" << '\n';
	}
}

void Player::initSprite()
{
	//Set the texture to the sprite
	sprite.setTexture(texture);

	//Resize  the sprite
	sprite.scale(0.2f, 0.2f);

}

 

//Con/Des
Player::Player()
{	
	

	initVariables();
	initTexture();
	initSprite();
}

Player::~Player()
{

}

const sf::Vector2f& Player::getPos() const
{		
	return sprite.getPosition();
}

const sf::FloatRect Player::getBounds() const
{
	return sprite.getGlobalBounds();
}

const int& Player::getHp() const
{
	return hp;
}

const int& Player::getHpMax() const
{
	return hpMax;
}

void Player::setPosition(const sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

void Player::setPosition(const float x, const float y)
{
	sprite.setPosition(x, y);
}

void Player::setHp(const int _hp)
{
	hp = _hp;
}

void Player::loseHp(const int value)
{
	hp -= value;
	if (hp < 0) { hp = 0; }

}

void Player::move(const float dirX, const float dirY)
{
	sprite.move(movementSpeed * dirX, movementSpeed * dirY);
}

const bool Player::canAttack()
{
	if (attackCooldown >= attackCooldownMax)
	{	
		attackCooldown = 0.f;
		return true;
	}

	return false;
}

void Player::updateAttack()
{
	if (attackCooldown < attackCooldownMax)
	{
		attackCooldown += 0.5f;
	}
	

}

//Functions
void Player::update()
{
	updateAttack();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}
