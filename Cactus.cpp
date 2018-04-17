#include "Cactus.h"
#include "Utils.h"

/// <summary>
/// Constructor
/// </summary>
Cactus::Cactus() :
	GameObj(0, 0, this->_width, this->_height) {
}

/// <summary>
/// Destructor
/// </summary>
Cactus::~Cactus() = default;

/// <summary>
/// Updates the cactus
/// </summary>
/// <param name="delta_time">Time since last frame</param>
void Cactus::onUpdate(double delta_time) {
	if (isDead()) return;
	const auto currentChange = _speed * static_cast<float>(delta_time);

	_x -= currentChange;

	if (_x + _w <= 0) {
		inflictDamage(1);
	}
}

/// <summary>
/// Renders the cactus' visuals
/// </summary>
/// <param name="render_target">Target to render to</param>
void Cactus::onRender(ID2D1HwndRenderTarget* render_target) {
	if (!render_target) return;

	//Drawing the enemies' visuals
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(_color, &brush);

	//Get rect for visuals
	const auto rect = getAABB();

	//Draw the lines
	render_target->FillRectangle(rect, brush);
	//render_target->DrawRectangle(get_aabb(), brush);

	Utils::safeRelease(&brush);
}

/// <summary>
/// Initializes the cactus
/// </summary>
void Cactus::initialize() {
	_color = D2D1::ColorF(D2D1::ColorF::Green, 1.0f);
	_speed = 250.0f;
	_health = 1;
	setLayer(LAYER::cactus);
}

/// <summary>
/// Handles the collisions with other objects
/// </summary>
/// <param name="collided_object">Object that this object collided with</param>
void Cactus::handleCollision(Transform2D* collided_object) {
	//Ignore collisions
}

/// <summary>
/// Returns the cactus' type
/// </summary>
/// <returns>Enemy type</returns>
Cactus::CACTUS_TYPE Cactus::getType() const {
	return _type;
}

/// <summary>
/// Sets the cactus' type
/// </summary>
/// <param name="type"></param>
void Cactus::setType(CACTUS_TYPE type) {
	_type = type;
	switch (_type) {
		case normal: {
			_w = 25;
			_h = 35;
			break;
		}
		case wide: {
			_w = 50;
			_h = 35;
			break;
		}
		case high: {
			_h = 50;
			_w = 25;
			break;
		}
	}
}
