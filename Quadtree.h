#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector>

#include "GameObject.h"

using namespace std;

class QuadTree {
    public:
	    QuadTree(float x, 
			float y, 
			float width,
			float height,
			int level, 
			int maxLevel, 
			QuadTree* parent);
       ~QuadTree();

	    vector<GameObj*> getObjectsAt(float x, float y, int layer = 0) const;
	    vector<GameObj*> getAllObjects() const;

		void addObject(GameObj *object);
	    void clear();
	    void update(const std::vector<GameObj*>& objects);
	    void render(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush);

	private:
		float _x;
		float _y;
		float _width;
		float _height;

		int	_level;
		int	_maxLevel;

		vector<GameObj*> _objects;

		QuadTree * _parent;
		QuadTree * _nw;
		QuadTree * _ne;
		QuadTree * _sw;
		QuadTree * _se;

		bool contains(QuadTree* child, GameObj* object);
		bool hasAnyLayer(GameObj* object, int layer) const;

		std::vector<GameObj*> getObjectsAtLayer(int layer) const;
};

#endif //QUADTREE_HPP
