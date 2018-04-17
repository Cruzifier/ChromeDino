#ifndef TRANSFORMTWOD_HPP
#define TRANSFORMTWOD_HPP
#include <d2d1.h>

class Transform2D {
	protected:
		float _x;
		float _y;
		float _w;
		float _h;

	public:
		/// <summary>
		/// Collision matrix to determine which layers should collide
		/// </summary>
		static bool collisionMatrix[4][4];

		enum LAYER {
			no_collisions = 0,
			player_bullet = 1,
			cactus = 2,
			player = 3
		};

		Transform2D(float x, float y, float width, float height);
		virtual ~Transform2D();

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;

		D2D1_RECT_F getAABB() const;

		void setSize(float width, float height);
		void setPos(float x, float y);
		void setLayer(LAYER layer);

		bool isColliding(Transform2D* other) const;

		LAYER getLayer() const;

	private:
		LAYER _layer;
};

#endif //TRANSFORMTWOD_HPP