#pragma once
#include "DllUtil.h"
#include "GameWorldObject.h"
#include "GameRegion.h"

libGameBackbone class GameWorldAnchor :
	public GameWorldObject {
public:

	//ctr / dtr
	GameWorldAnchor();
	explicit GameWorldAnchor(GameRegion* anchoredRegion);
	GameWorldAnchor(GameRegion* anchoredRegion, double gx, double gy);
	virtual ~GameWorldAnchor();

	//getters / setters
	
		//setters
	void setAnchoredRegion(GameRegion* region);
	GameRegion* getAnchoredRegion();

	//operations
	virtual void gMove(double xOffset, double yOffset);
	virtual void setActive(bool active);

protected:
	GameRegion* anchoredRegion;
};

