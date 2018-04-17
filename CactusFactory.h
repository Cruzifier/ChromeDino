#ifndef CACTUSFACTORY_HPP
#define CACTUSFACTORY_HPP

#include "Cactus.h"

class Logic;

class CactusFactory {
	private:
		ID2D1PathGeometry *	_triangleGeometry;
		ID2D1PathGeometry *	_hexagonGeometry;
		Logic*				_logic;

	public:
		CactusFactory(Logic* game);
		~CactusFactory();

		Cactus*	make_cactus(Cactus::CACTUS_TYPE type, float x, float y) const;

		void initialize();
};

#endif //CACTUSFACTORY_HPP