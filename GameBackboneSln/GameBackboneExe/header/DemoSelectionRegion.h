#pragma once

#include <DemoRegion.h>

namespace EXE {

	/// <summary>
	/// 
	/// </summary>
	class DemoSelectionRegion : public DemoRegion
	{
	public:
		// Ctrs / Dtr
		DemoSelectionRegion(sf::RenderWindow & window);

		DemoSelectionRegion() = delete;
		DemoSelectionRegion(const DemoSelectionRegion& other) = delete;
		DemoSelectionRegion(DemoSelectionRegion&& other) = delete;
		DemoSelectionRegion& operator= (const DemoSelectionRegion& other) = delete;
		DemoSelectionRegion& operator= (DemoSelectionRegion&& other) = delete;

		~DemoSelectionRegion();

		// internal behavior alteration
		virtual void registerSetActiveRegionCB(std::function<void(GameRegion*)> newSetActiveRegionCB) override;
	
	private:

	protected:

		// initialization
		void initGUI();

		// gui callbacks
		void navigationRegionCB();
		void swirlyDemoCB();

	};

}

