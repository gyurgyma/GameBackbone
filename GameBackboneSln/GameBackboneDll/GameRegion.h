#pragma once
#include "stdafx.h"
#include "AnimatedSprite.h"
#include "BackboneBaseExceptions.h"
#include "CompoundSprite.h"
#include "DllUtil.h"
#include "Updatable.h"

#include <TGUI\TGUI.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include<functional>
#include <list>

namespace GB {

	/// <summary> Base class meant to be inherited. Controls game logic and actors for a specific time or space in game. </summary>
	class libGameBackbone GameRegion {
	public:
		//ctr / dtr
		GameRegion();
		GameRegion(sf::RenderWindow & window);

		virtual ~GameRegion();

		//getters / setters
		//setters
		void setUpdatable(bool status, Updatable* object);
		void setDrawable(bool status, sf::Sprite* object);
		void setDrawable(bool status, CompoundSprite* object);
		void setDrawAndUpdateable(bool status, AnimatedSprite* object);
		void setDrawAndUpdateable(bool status, CompoundSprite* object);
		void setParentRegion(GameRegion* newParent);

		//getters
		std::list<Updatable*>* getUpdatables();
		std::list<sf::Sprite*>* getDrawables();
		std::list<GameRegion*>* getNeighborRegions();
		std::list<GameRegion*>* getChildRegions();
		GameRegion* getParentRegion();
		tgui::Gui* getGUI();


		//internal behavior alteration

		/// <summary>
		/// Resets this instance.
		/// </summary>
		virtual void reset() {}

		/// <summary>
		/// Runs the game behaviors and logic for this instance.
		/// </summary>
		virtual void behave(sf::Time currentTime) {}

		//general operations

		//additions
		void addChildRegion(GameRegion* childToAdd);
		void addNeighborRegion(GameRegion* neighborToAdd);
		//removals
		void removeNeighborRegion(GameRegion* neighborToRemove);
		void removeChildRegion(GameRegion* childToRemove);
		//clears
		void clearUpdatable();
		void clearChildren();
		void clearDrawable();
		void clearNeighborRegions();

	protected:

		//ctr
		void init() {}

		//operations
		void clearAssociations(std::function<void(GameRegion*)> memberFunctionPointer, std::list<GameRegion*>* list);

		//internal logic members
		std::list<sf::Sprite*>* drawables;
		std::list<Updatable*>* updatables;

		//associated regions
		GameRegion* parentRegion;
		std::list<GameRegion*> childRegions;
		std::list<GameRegion*> neighborRegions;

		//GUI
		tgui::Gui* regionGUI;

	private:
		//deleted copy and assignment ctr
		GameRegion(const GameRegion&) = delete;
		GameRegion& operator=(const GameRegion&) = delete;
		GameRegion(GameRegion&&) = delete;
		GameRegion& operator=(GameRegion&&) = delete;

	};

}
