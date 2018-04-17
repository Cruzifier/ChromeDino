#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <d2d1.h>

#include "Player.h"
#include "Quadtree.h"
#include "CactusFactory.h"

class ChromeDino;

class Logic {
	public:
		Logic(ChromeDino* game);
		~Logic();

		void initialize();
		void onRender(ID2D1HwndRenderTarget* renderTarget, IDWriteTextFormat* textFormat);

		bool onUpdate(double delta);

		ChromeDino* getDino() const;

	private:
		Player				    _player;
		QuadTree			    _quadTree;
		std::vector<GameObj*>	_objects;
		CactusFactory		    _cactusFactory;
		ChromeDino*				_game;

		float					_timeSinceSpawn;
		float					_minSpawnSpeed;
		float					_maxSpawnSpeed;
		float				    _points;

		void checkCollisions();
		void cleanup(bool end = false);
		void createCactus(Cactus::CACTUS_TYPE type, float x, float y);
		void onUpdateSpawn(const float delta);
};

#endif //LOGIC_HPP