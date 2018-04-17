#include "Player.h"
#include "Input.h"
#include "Logic.h"
#include "Utils.h"
#include "Resolution.h"

/// <summary>
/// Constructor
/// </summary>
Player::Player(Logic* logic) : 
	GameObj			(0, 0, 30.0f, 50.0f),
	_logic				(logic),
	_yVelocity			(0) {}

/// <summary>
/// Destructor
/// </summary>
Player::~Player() = default;

/// <summary>
/// Updates the player's position
/// </summary>
/// <param name="delta_time">Time since last update</param>
void Player::onUpdate(double delta_time) {
	if (isDead()) return;

	if (Input::getInstance().isKeyDown(Input::Space) && _y >= HEIGHT) {
		//Jump
		_yVelocity = -5;
		_isJumping = true;
	}

	_y += _yVelocity;
	if(_y >= HEIGHT && !_isJumping) {
		_y = HEIGHT;
		if(_yVelocity < 0) {
			_yVelocity = 0.0f;
		}
	} else {
		_yVelocity -= GRAVITY * delta_time;
	}
	_isJumping = false;

	
}

/// <summary>
/// Renders the player
/// </summary>
/// <param name="render_target">Target to render to</param>
void Player::onRender(ID2D1HwndRenderTarget* renderTarget) {
	if (!renderTarget) return;

	//Drawing the player's visuals
	ID2D1SolidColorBrush* brush;
	renderTarget->CreateSolidColorBrush(_color, &brush);


	//Draw the lines
	renderTarget->FillRectangle(getAABB(), brush);
	//render_target->DrawRectangle(get_aabb(), brush);

	Utils::safeRelease(&brush);
}

/// <summary>
/// Initializes the player's variables
/// </summary>
void Player::initialize() {
	_color = D2D1::ColorF(D2D1::ColorF::White, 1.0f);
	_speed = 400.0f;
	_health = 1;
	setLayer(LAYER::player);
}

/// <summary>
/// Handles the collisions with other objects
/// </summary>
/// <param name="collided_object">Object that this object collided with</param>
void Player::handleCollision(Transform2D* collidedObject) {
	inflictDamage(1);
}
