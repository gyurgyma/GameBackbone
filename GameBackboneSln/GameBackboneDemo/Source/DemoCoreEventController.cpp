#include <GameBackboneDemo/DemoCoreEventController.h>
#include <GameBackboneDemo/NavigationDemoRegion.h>

using namespace EXE;

/// <summary>
/// Initializes a new instance of the <see cref="BackboneExeCoreEventController"/> class. Window width, height, and name are default.
/// </summary>
DemoCoreEventController::DemoCoreEventController() : CoreEventController("GameBackbone Demos") {
	// Initialize the MainMenuDemoRegion
	mainMenuDemoRegion= new MainMenuDemoRegion(*window);
	// Register setActiveRegion with mainMenuDemoRegion so that it can swap regions when needed
	mainMenuDemoRegion->registerSetActiveRegionCB(std::bind(&DemoCoreEventController::setActiveRegion, this, std::placeholders::_1));
	// Set mainMenuDemoRegion as the activeRegion on DemoCoreEventController
	activeRegion = mainMenuDemoRegion;

	// Set the camera to the same as the window
	camera.reset(sf::FloatRect(0, 0, (float)window->getSize().x, (float)window->getSize().y));
}

/// <summary>
/// Finalizes an instance of the <see cref="BackboneExeCoreEventController"/> class.
/// </summary>
DemoCoreEventController::~DemoCoreEventController() {
	delete mainMenuDemoRegion;
}

//events

/// <summary>
/// Handles non gui user/window events.
/// </summary>
/// <param name="event">The event.</param>
/// <returns></returns>
bool DemoCoreEventController::handleCoreEvent(sf::Event & event) {
	// Handle events not handled by the GUI
	switch (event.type) {
	case sf::Event::Closed:
	{
		// Close the window, thus closing the game.
		window->close();
		return true;
	}
	case sf::Event::MouseMoved:
	{
		// Get the pixel position and map it to coordinates
		sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
		sf::Vector2f actualPosition = window->mapPixelToCoords(mousePos);
		// Pass the event to the active region to handle
		static_cast<DemoRegion*>(activeRegion)->handleMouseMove(actualPosition);
		return true;
	}
	case sf::Event::MouseButtonPressed:
	{
		// Get the pixel position and map it to coordinates
		sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
		sf::Vector2f actualPosition = window->mapPixelToCoords(mousePos);
		// Pass the event to the active region to handle
		static_cast<DemoRegion*>(activeRegion)->handleMouseClick(actualPosition, event.mouseButton.button);
		return true;
	}
	case sf::Event::MouseWheelScrolled:
	{
		// Pass the event to the active region to handle
		static_cast<DemoRegion*>(activeRegion)->handleWheelScroll(event.mouseWheelScroll.delta);
		return true;
	}
	case sf::Event::KeyPressed:
	{
		// Pass the event to the active region to handle
		static_cast<DemoRegion*>(activeRegion)->handleKeyPress(event.key);
		return true;
	}
	case sf::Event::KeyReleased:
	{
		// Pass the event to the active region to handle
		static_cast<DemoRegion*>(activeRegion)->handleKeyRelease(event.key);
		return true;
	}
	case sf::Event::Resized:
	{
		// Reset the camera to the same as the window
		camera.reset(sf::FloatRect(0, 0, (float)event.size.width, (float)event.size.height));
		// Set the view on the window to be the reset camera
		window->setView(camera);
		// Set the view on the GUI to be the reset camera
		activeRegion->getGUI()->setView(camera);
		return true;
	}
	default:
	{
		return false;
	}
	}

}