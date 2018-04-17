#ifndef DINO_CACTUS_HPP
#define DINO_CACTUS_HPP

#include "GameObject.h"

class Cactus : public GameObj {
	public:
		enum CACTUS_TYPE {
			normal = 0,
			wide = 1,
			high = 2
		};

		Cactus();
		~Cactus();

		void onUpdate(double deltaTime) override;
		void onRender(ID2D1HwndRenderTarget* renderTarget) override;
		void initialize() override;
		void handleCollision(Transform2D* collidedObject) override;

		virtual CACTUS_TYPE	getType() const;
		void setType(CACTUS_TYPE type);

	private:
		int _width  = 25;
		int _height = 25;
		CACTUS_TYPE								_type;
	};


#endif //DINO_CACTUS_HPP

