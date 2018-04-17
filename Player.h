#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GameObject.h"

#define GRAVITY	-9.81f

class Logic;

class Player : public GameObj {
    public:
	    explicit Player(Logic* logic);
	    ~Player();

	    void onUpdate(double deltaTime) override;
	    void onRender(ID2D1HwndRenderTarget* renderTarget) override;
	    void initialize() override;
	    void handleCollision(Transform2D* collidedObject) override;

	private:
		Logic* _logic;

		float  _yVelocity;
		bool  _isJumping;
};

#endif //PLAYER_HPP