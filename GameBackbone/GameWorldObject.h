#pragma once
#include "DllUtil.h"
#include "Updatable.h"
#include "Point2D.h"

libGameBackbone class GameWorldObject {
public:

	//ctr / dtr
	GameWorldObject();
	virtual ~GameWorldObject();

	//getters / setters

		//setters
	///<summary>Whether or not the GameWorldObject is active in the game world</summary>
	virtual void setActive(bool active) = 0;

		//getters
	double getGx();
	double getGy();
	double isActive();

	//operations
	///<summary>Changes the position of the sprite in the game world by the given offsets</summary>
	virtual void gMove(double xOffset, double yOffset) = 0;

protected:
	Point2D gwPosition;
	bool active;
};

