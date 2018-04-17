#include "CactusFactory.h"
#include "ChromeDino.h"
#include "Utils.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="game">The game logic instance</param>
CactusFactory::CactusFactory(Logic* game) :
	_triangleGeometry		(nullptr),
	_logic				(game){
}

/// <summary>
/// Destructor
/// </summary>
CactusFactory::~CactusFactory() = default;

/// <summary>
/// Initializes the factory
/// </summary>
void CactusFactory::initialize() {
	if (!_logic) return;
	const auto factory = _logic->getDino()->getDirect2dFactory();

	if (!factory) return;
	
	ID2D1GeometrySink* sink;

	//Create the triangle geometry
	auto hr = factory->CreatePathGeometry(&_triangleGeometry);
	if (SUCCEEDED(hr)) {
		//Write to the path geometry using the geometry sink.
		hr = _triangleGeometry->Open(&sink);

		if (SUCCEEDED(hr)) {

			sink->BeginFigure(
				D2D1::Point2F(0, 0),
				D2D1_FIGURE_BEGIN_FILLED
			);

			//Add triangle lines
			sink->AddLine(D2D1::Point2F(20, 0));
			sink->AddLine(D2D1::Point2F(10, 20));
			sink->AddLine(D2D1::Point2F(0, 0));

			sink->EndFigure(D2D1_FIGURE_END_CLOSED);

			sink->Close();
		}
		Utils::safeRelease(&sink);
	}

	//Create the hexagon geometry
	hr = factory->CreatePathGeometry(&_hexagonGeometry);
	if (SUCCEEDED(hr)) {
		//Write to the path geometry using the geometry sink.
		hr = _hexagonGeometry->Open(&sink);

		if (SUCCEEDED(hr)) {

			sink->BeginFigure(
				D2D1::Point2F(8, 0),
				D2D1_FIGURE_BEGIN_FILLED
			);

			//Add hexagon lines
			sink->AddLine(D2D1::Point2F(16, 0));
			sink->AddLine(D2D1::Point2F(24, 8));   
			sink->AddLine(D2D1::Point2F(24, 16));
			sink->AddLine(D2D1::Point2F(16, 24));
			sink->AddLine(D2D1::Point2F(8, 24));
			sink->AddLine(D2D1::Point2F(0, 16));
			sink->AddLine(D2D1::Point2F(0, 8));
			sink->AddLine(D2D1::Point2F(8, 0));

			sink->EndFigure(D2D1_FIGURE_END_CLOSED);

			sink->Close();
		}
		Utils::safeRelease(&sink);
	}
	
}

/// <summary>
/// Creates a new cactus of the specified type
/// </summary>
/// <param name="type">Type of cactus</param>
/// <param name="x">x Position</param>
/// <param name="y">y Position</param>
/// <returns></returns>
Cactus* CactusFactory::make_cactus(const Cactus::CACTUS_TYPE type, const float x, const float y) const {
	Cactus* newCactus = new Cactus();
	if (newCactus) {
		newCactus->initialize();
		newCactus->setPos(x, y);
		newCactus->setType(type);
	}
	return newCactus;
}
