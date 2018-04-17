#include <algorithm>
#include <sstream>
#include <iomanip>

#include "Logic.h"
#include "ChromeDino.h"
#include "Resolution.h"
#include "Utils.h"


/// <summary>
/// Constructor
/// </summary>
Logic::Logic(ChromeDino* game):
	_player			    (this),
	_quadTree		    (0.0f, 0.0f, WIDTH, HEIGHT, 0, 2, nullptr), 
	_cactusFactory      (this),
	_game			    (game),
	_timeSinceSpawn		(0.0f),
	_minSpawnSpeed		(1.0f),
	_maxSpawnSpeed		(2.0f),
	_points			    (0) { }

/// <summary>
/// Destructor
/// </summary>
Logic::~Logic() = default;

/// <summary>
/// Initializes the game
/// </summary>
void Logic::initialize() {
	_player.initialize();
	_player.setPos(40, HEIGHT - 200);

	_quadTree.addObject(&_player);

	_cactusFactory.initialize();
}

/// <summary>
/// Renders all the game's visuals
/// </summary>
/// <param name="render_target">Target to render to</param>
/// <param name="text_format">Format for writing texts</param>
void Logic::onRender(ID2D1HwndRenderTarget* render_target, IDWriteTextFormat* text_format) {
	//Render the player
	_player.onRender(render_target);

	//Render objects
	for (auto& obj : _objects) {
		obj->onRender(render_target); 
	}

	//Create brush
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
	

	//Create string to display
	std::stringstream ss;
	ss << "Score: ";
	ss << std::setw(4) << std::setfill('0') << static_cast<int>(_points);

	wstring widestr;
	for (auto i = 0; static_cast<int>(i) < static_cast<int>(ss.str().length()); ++i) {
		widestr += wchar_t(ss.str()[i]);
	}

	const auto str = widestr.c_str();

	//Render kills
	render_target->DrawText(
		str,
		ss.str().length(),
		text_format,
		D2D1::RectF(0, 0, 150, 50),
		brush
	);

	Utils::safeRelease(&brush);
}

/// <summary>
/// Updates all the game's states and logic
/// </summary>
/// <param name="delta">Time since last frame in seconds</param>
/// <returns>True if game has ended</returns>
bool Logic::onUpdate(const double delta) {
	onUpdateSpawn(delta);

	_quadTree.update(_objects);
	_quadTree.addObject(&_player);

	_player.onUpdate(delta);

	_points += delta * 4;

	//Update enemies
	for (size_t i = 0; i < _objects.size(); i++) {
		_objects[i]->onUpdate(delta);
	}

	checkCollisions();

	if(_player.isDead()) {
		//Player is dead, game ended
		return true;
	}

	cleanup();
	return false;
}


/// <summary>
/// Checks for collisions and handles them
/// </summary>
void Logic::checkCollisions() {
	//Check if the object can collide at all
	if (_player.getLayer() != 0) {
		auto near_objects = _quadTree.getObjectsAt(_player.getX(), _player.getY() - 1);
		for (auto& near_object : near_objects) {
			if (near_object != &_player && near_object->isColliding(&_player)) {
				//Objects are colliding, so let them handle it
				_player.onCollision(near_object);
			}
		}
	}
}

/// <summary>
/// Cleans up dead objects
/// </summary>
/// <param name="end">True if game ended, false if not</param>
void Logic::cleanup(bool end) {
	if(end) {
		for (auto& _object : _objects) {
			delete _object;
		}
		return;
	}
	//Delete dead objects
	_objects.erase(std::remove_if(_objects.begin(), _objects.end(), [this](GameObj* obj) {
		if(obj->isDead()) {
			delete obj;
			obj = nullptr;
			return true;
		};
		return false;
	}), _objects.end());
}

/// <summary>
/// Creates a new cactus of the specified type
/// </summary>
/// <param name="type">Type of cactus</param>
/// <param name="x">x Position</param>
/// <param name="y">y Position</param>
/// <returns></returns>
void Logic::createCactus(Cactus::CACTUS_TYPE type, float x, float y) {
	const auto enemy = _cactusFactory.make_cactus(type, x, y);
	_objects.push_back(enemy);
}

/// <summary>
/// Updates the spawning process
/// </summary>
/// <param name="delta">Time since last frame</param>
void Logic::onUpdateSpawn(const float delta) {
	//Try to spawn new enemies
	if (_timeSinceSpawn > 0.0f) {
		_timeSinceSpawn -= delta;
	}

	//Spawn enemies
	if (_timeSinceSpawn <= 0.0f) {
		_timeSinceSpawn =  _minSpawnSpeed + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (_maxSpawnSpeed - _minSpawnSpeed)));

		//Cacti spawn with different probabilities
		//60% Normal
		//20% wide
		//20% High
		Cactus::CACTUS_TYPE type = Cactus::CACTUS_TYPE::normal;
		const int percent = (rand() % 99) + 1;
		if (percent > 60 && percent <= 80) {
			type = Cactus::CACTUS_TYPE::wide;
		} else if(percent > 80) {
			type = Cactus::CACTUS_TYPE::high;
		}

		createCactus(type, WIDTH, HEIGHT - 1);
	}
}

/// <summary>
/// Returns the game instance
/// </summary>
/// <returns>Game pointer</returns>
ChromeDino* Logic::getDino() const {
	return _game;
}
