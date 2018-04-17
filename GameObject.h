#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <d2d1.h>

#include "Transform2d.h"

class QuadTree;

class GameObj : public Transform2D {
	protected:
		ID2D1SolidColorBrush * _brush;
		D2D1::ColorF		   _color;
		float				   _speed;
		int					   _health;

		virtual void handleCollision(Transform2D* collidedObject) = 0;

	public:
		GameObj(float x, float y, float width, float height);
		virtual ~GameObj();

		virtual void onUpdate(double deltaTime) = 0;
		virtual void onRender(ID2D1HwndRenderTarget* renderTarget) = 0;
		virtual void initialize() = 0;
		
		void onCollision(Transform2D* collidedObject);
		void setSpeed(float speed);
		void setColor(D2D1::ColorF color);

		bool inflictDamage(int damage);
		bool isDead() const;

		int	getHealth() const;
};

#endif //GAMEOBJECT_HPP