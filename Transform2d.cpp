#include "Transform2d.h"

bool Transform2D::collisionMatrix[4][4] = {
	false,	false,	false,	false,
	false,	false,	true,	false,
	false,	true,	false,	true,
	false,	false,	true,	false,
};

/// <summary>
/// Constructor
/// </summary>
/// <param name="x">Position on the x axis</param>
/// <param name="y">Position on the y axis</param>
/// <param name="width">Width of the object</param>
/// <param name="height">Height of the object</param>
Transform2D::Transform2D(const float x, const float y, const float width, const float height):
	_x			(x),
	_y			(y),
	_w		(width),
	_h		(height) {
	
}

/// <summary>
/// Destructor
/// </summary>
Transform2D::~Transform2D() = default;

/// <summary>
/// Returns the x position
/// </summary>
/// <returns></returns>
float Transform2D::getX() const { return _x; }

/// <summary>
/// Returns the y position
/// </summary>
/// <returns></returns>
float Transform2D::getY() const { return _y; }

/// <summary>
/// Returns the width
/// </summary>
/// <returns></returns>
float Transform2D::getWidth() const { return _w; }

/// <summary>
/// Returns the height
/// </summary>
/// <returns></returns>
float Transform2D::getHeight() const { return _h; }

/// <summary>
/// Returns the axis-aligned bounding box of this object
/// </summary>
/// <returns>Rect</returns>
D2D1_RECT_F Transform2D::getAABB() const {
	const auto rect = D2D1::RectF(_x, _y - _h, _x + _w, _y);
	return rect;
}

/// <summary>
/// Sets the size of this object
/// </summary>
/// <param name="width">Width of the object</param>
/// <param name="height">Height of the object</param>
void Transform2D::setSize(const float width, const float height) {
	_w = width;
	_h = height;
}

/// <summary>
/// Sets the position of this object
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void Transform2D::setPos(const float x, const float y) {
	_x = x;
	_y = y;
}

/// <summary>
/// Returns true if the transforms are colliding, false if not
/// </summary>
/// <param name="other">Transform to check against</param>
/// <returns></returns>
bool Transform2D::isColliding(Transform2D* other) const {
	if (other == nullptr) return false;
	const auto otherAABB = other->getAABB();
	const auto myAABB = getAABB();

	//Collision tests
	if (myAABB.right < otherAABB.left || myAABB.left > otherAABB.right) return false;
	if (myAABB.top > otherAABB.bottom || myAABB.bottom < otherAABB.top) return false;

	return true;
}

/// <summary>
/// Sets this object's layer
/// </summary>
/// <param name="layer">Layer to set</param>
void Transform2D::setLayer(const LAYER layer) {
	_layer = layer;
}

/// <summary>
/// Returns this object's layer
/// </summary>
/// <returns>Layer of the object</returns>
Transform2D::LAYER Transform2D::getLayer() const {
	return _layer;
}
