#include <iostream>
#include <sstream>

#include "Quadtree.h"
#include "Utils.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="x">Position on the x axis</param>
/// <param name="y">Position on the y axis</param>
/// <param name="width">Width of the tree</param>
/// <param name="height">Height of the tree</param>
/// <param name="level">Level of the tree</param>
/// <param name="maxLevel">Maximum level of the tree</param>
QuadTree::QuadTree(float x, float y, float width, float height, int level, int maxLevel, QuadTree* parent) :
	_x			(x),
	_y			(y),
	_width		(width),
	_height		(height),
	_level		(level),
	_maxLevel	(maxLevel),
	_parent		(parent)
{
	if (level == maxLevel) {
		return;
	}

	_nw = new QuadTree(x, y, width / 2.0f, height / 2.0f, level+1, maxLevel, this);
	_ne = new QuadTree(x + width / 2.0f, y, width / 2.0f, height / 2.0f, level+1, maxLevel, this);
	_sw = new QuadTree(x, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, maxLevel, this);
	_se = new QuadTree(x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, maxLevel, this);
}

/// <summary>
/// Destructor
/// </summary>
QuadTree::~QuadTree() {
	if (_level == _maxLevel)
		return;

	delete _nw;
	delete _ne;
	delete _sw;
	delete _se;
}

/// <summary>
/// Adds a new object to the tree
/// </summary>
/// <param name="object">Object to add</param>
void QuadTree::addObject(GameObj* object) {
	if (_level == _maxLevel) {
		_objects.push_back(object);
		return;
	}
	if (contains(_nw, object)) {
		_nw->addObject(object); return;
	} else if (contains(_ne, object)) {
		_ne->addObject(object); return;
	} else if (contains(_sw, object)) {
		_sw->addObject(object); return;
	} else if (contains(_se, object)) {
		_se->addObject(object); return;
	}
	if (contains(this, object)) {
		_objects.push_back(object);
	}
}

/// <summary>
/// Returns the objects in the quadtree that are inside the area with the given point
/// </summary>
/// <param name="x">Position on the x axis</param>
/// <param name="y">Position on the y axis</param>
/// <param name="layer">Accepted layers of the object</param>
/// <returns>List of collision objects</returns>
vector<GameObj*> QuadTree::getObjectsAt(float x, float y, int layer) const {
	if (_level == _maxLevel) {
		if(layer == 0) {
			return _objects;
		}
		return getObjectsAtLayer(layer);
	}

	vector<GameObj*> returnObjects, childReturnObjects;
	if (!_objects.empty()) {
		if(layer == 0) {
			returnObjects = _objects;
		} else {
			returnObjects = getObjectsAtLayer(layer);
		}
	}

	//Check each subtree and add results
	if (x > _x + _width / 2.0f && x <= _x + _width) {
		if (y > _y + _height / 2.0f && y <= _y + _height) {
			childReturnObjects = _se->getObjectsAt(x, y);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
			return returnObjects;
		}
		if (y > _y && y <= _y + _height / 2.0f) {
			childReturnObjects = _ne->getObjectsAt(x, y);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
			return returnObjects;
		}
	} else if (x > _x && x <= _x + _width / 2.0f) {
		if (y > _y + _height / 2.0f && y <= _y + _height) {
			childReturnObjects = _sw->getObjectsAt(x, y);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
			return returnObjects;
		}
		if (y > _y && y <= _y + _height / 2.0f) {
			childReturnObjects = _nw->getObjectsAt(x, y);
			returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
			return returnObjects;
		}
	}
	return returnObjects;
}

/// <summary>
/// Returns all the objects in the tree
/// </summary>
/// <returns></returns>
vector<GameObj*> QuadTree::getAllObjects() const {
	if (_level == _maxLevel) {
		return _objects;
	}

	vector<GameObj*> returnObjects;
	if (!_objects.empty()) {
		returnObjects = _objects;
	}

	auto childReturnObjects = _se->getAllObjects();
	returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
	
	childReturnObjects = _ne->getAllObjects();
	returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());
	
	childReturnObjects = _sw->getAllObjects();
	returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());

	childReturnObjects = _nw->getAllObjects();
	returnObjects.insert(returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end());

	return returnObjects;
}

/// <summary>
/// Clears the tree
/// </summary>
void QuadTree::clear() {
	if (_level == _maxLevel) {
		_objects.clear();
		return;
	}
	_nw->clear();
	_ne->clear();
	_sw->clear();
	_se->clear();

	if (!_objects.empty()) {
		_objects.clear();
	}
}

/// <summary>
/// Updates the quadtree, so that moved objects are sorted correctly
/// </summary>
void QuadTree::update(const std::vector<GameObj*>& objects) {
	//Only allow calling on the root
	if (_parent != nullptr) return;
	
	//Rebuild the tree
	clear();
	for (auto* object : objects) {
		addObject(object);
	}
}

/// <summary>
/// Renders the quadtree
/// </summary>
/// <param name="render_target">Target to render to</param>
void QuadTree::render(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush) {
	if (renderTarget == nullptr) return;
	if(brush == nullptr) {
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::AntiqueWhite), &brush);
	}
	if(_level == _maxLevel) {
		renderTarget->DrawRectangle(D2D1::RectF(_x, _y, _x + _width, _y + _height), brush);
		return;
	}
	_se->render(renderTarget, brush);
	_ne->render(renderTarget, brush);
	_nw->render(renderTarget, brush);
	_sw->render(renderTarget, brush);

	if (_parent == nullptr) {
		Utils::safeRelease(&brush);
	}
}

/// <summary>
/// Returns true if the tree contains the given object, false if not
/// </summary>
/// <param name="child">Tree to check</param>
/// <param name="object">Object to check</param>
/// <returns></returns>
bool QuadTree::contains(QuadTree *child, GameObj *object) {
	if (child == nullptr || object == nullptr) return false;
	//Classic aabb collision check
	return	 !(object->getX() < child->_x ||
				object->getY() < child->_y ||
				object->getX() > child->_x + child->_width  ||
				object->getY() > child->_y + child->_height ||
				object->getX() + object->getWidth() < child->_x ||
				object->getY() - object->getHeight() < child->_y ||
				object->getX() + object->getWidth() > child->_x + child->_width ||
				object->getY() - object->getHeight() > child->_y + child->_height);
}

/// <summary>
/// Returns true if the object has a layer contained in the given layer
/// </summary>
/// <param name="object">Object to test layer of</param>
/// <param name="layer">Layers to check</param>
/// <returns></returns>
bool QuadTree::hasAnyLayer(GameObj* object, int layer) const {
	if (object == nullptr) return false;
	const auto objLayer = object->getLayer();

	//Test if bit is set
	return ((objLayer & layer) == objLayer);
}

/// <summary>
/// Returns a vector of all the objects that match the given layers
/// </summary>
/// <param name="layer">Layers to check for</param>
/// <returns></returns>
std::vector<GameObj*> QuadTree::getObjectsAtLayer(int layer) const {
	vector<GameObj*> returnObjects;
	for (auto object : _objects) {
		if (hasAnyLayer(object, layer)) {
			returnObjects.push_back(object);
		}
	}
	return returnObjects;
}
