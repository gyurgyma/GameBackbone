#include <Backbone\BackboneBaseExceptions.h>
#include <Navigation\NavigationTools.h>
#include <Util\UtilMath.h>

#include <SFML\Graphics\Sprite.hpp>

#include <math.h>
#include <memory>

using namespace GB;




/// <summary>
/// Frees all memory stored in the NavigationGrid
/// </summary>
/// <param name="navGrid">The NavigationGrid.</param>
void GB::freeAllNavigationGridData(NavigationGrid& navGrid) {
	for (unsigned int ii = 0; ii < navGrid.getArraySizeX(); ++ii) {
		for (unsigned int jj = 0; jj < navGrid.getArraySizeY(); ++jj)
		{
			delete navGrid[ii][jj];
		}
	}
}


/// <summary>
/// Moves the sprite in the direction of the destination.
/// The sprite will never overshoot the destination.
/// </summary>
/// <param name="sprite">The sprite to move.</param>
/// <param name="destination">The destination.</param>
/// <param name="maxStepLength">Maximum length that the sprite can move.</param>
/// <param name="orientSpriteToDestination">Orients sprites towards their destination if true. Does not orient sprites otherwise.</param>
void GB::moveSpriteStepTowardsPoint(sf::Sprite& sprite,
									const sf::Vector2f& destination,
									const float maxStepLength,
									const bool orientSpriteToDestination) {
	std::vector<sf::Sprite*> sprites = {&sprite};
	std::vector<sf::Vector2f> destinations = {destination};
	std::vector<float> maxStepLengths = {maxStepLength};

	bulkMoveSpriteStepTowardsPoint(sprites, destinations, maxStepLengths, orientSpriteToDestination);
}




/// <summary>
/// Moves any movable object a step towards a destination.
/// </summary>
/// <param name="movableObject">The movable object.</param>
/// <param name="destination">The destination.</param>
/// <param name="maxStepLength">Maximum length of the step.</param>
/// <param name="distanceToDestination">Out Value: The distance to destination.</param>
/// <param name="angleToDest">Out Value: The angle to the destination.</param>
template <class T>
static inline void moveMovableStepTowardsPoint(T* movableObject,
											   const sf::Vector2f& destination,
											   const float maxStepLength, 
											   float& distanceToDestination,
											   float& angleToDest) {

	// calculate the angle to the destination
	const sf::Vector2f currentPosition = movableObject->getPosition();
	angleToDest = atan2f(destination.y - currentPosition.y, destination.x - currentPosition.x);

	// Calculate the distance to the destination
	distanceToDestination = CalcDistance2D_2<float, sf::Vector2f>(currentPosition, destination);

	// Move directly to the destination if it's within reach
	if (distanceToDestination <= maxStepLength) {
		movableObject->setPosition(destination);
	}
	else { // Move the sprite as close as possible to the destination
		float xProgress = cosf(angleToDest) * maxStepLength;
		float yProgress = sinf(angleToDest) * maxStepLength;

		movableObject->move(xProgress, yProgress);
	}
}



/// <summary>
/// Moves all passed sprites towards the destination of the same index.
/// Sprites will not overshoot their destination.
/// </summary>
/// <param name="sprites">The sprites.</param>
/// <param name="destinations">The destinations.</param>
/// <param name="maxStepLengths">The maximum distances to move the sprites.</param>
/// <param name="orientSpritesToDestination">Orients sprites towards their destination if true. Does not orient sprites otherwise.</param>
void GB::bulkMoveSpriteStepTowardsPoint(const std::vector<sf::Sprite*>& sprites,
										const std::vector<sf::Vector2f>& destinations,
										const std::vector<float>& maxStepLengths,
										const bool orientSpritesToDestination) {

	// ensure that all arrays are the same size
	if (sprites.size() != destinations.size() || sprites.size() != maxStepLengths.size()) {
		throw Error::NavigationTools_MismatchedNavigationSizes();
	}

	for (unsigned int ii = 0; ii < sprites.size(); ii++) {
		sf::Sprite* sprite = sprites[ii];
		sf::Vector2f destination = destinations[ii];
		float maxStepLength = maxStepLengths[ii];
		
		// move the sprite to
		float angleToDest;
		float distanceToDestination;

		// Move the sprite. Find its distance to destination and its angle to the destination.
		moveMovableStepTowardsPoint(sprite, destination, maxStepLength, distanceToDestination, angleToDest);

		// rotate the sprite if rotation is on
		if (orientSpritesToDestination && distanceToDestination != 0) {
			sprite->setRotation(angleToDest * 180.0f / (float)M_PI);
		}
	}
}



/// <summary>
/// Moves the CompoundSprite in the direction of the destination.
/// The sprite will never overshoot the destination.
/// </summary>
/// <param name="sprite">The CompoundSprite to move.</param>
/// <param name="destination">The destination.</param>
/// <param name="maxStepLength">Maximum length that the sprite can move.</param>
/// <param name="orientSpriteToDestination">Orients sprites towards their destination if true. Does not orient sprites otherwise.</param>
void moveCompoundSpriteStepTowardsPoint(CompoundSprite& sprite,
	const sf::Vector2f& destination,
	const float maxStepLength,
	const std::set<size_t>& spritesToRotate) {

}

/// <summary>
/// Moves all passed CompoundSprites towards the destination of the same index.
/// Sprites will not overshoot their destination.
/// </summary>
/// <param name="sprites">The sprites.</param>
/// <param name="destinations">The destinations.</param>
/// <param name="maxStepLengths">The maximum distance to move the sprites.</param>
/// <param name="spritesToRotate">The sprites to rotate.</param>
void bulkMoveCompoundSpriteStepTowardsPoint(const std::vector<CompoundSprite*>& sprites,
											const std::vector<sf::Vector2f>& destinations,
											const std::vector<float>& maxStepLengths,
											const std::vector<std::set<size_t>>& spritesToRotate)
{
	// ensure that all arrays are the same size
	if (sprites.size() != destinations.size() || sprites.size() != maxStepLengths.size() || sprites.size() != spritesToRotate.size()) {
		throw Error::NavigationTools_MismatchedNavigationSizes();
	}

	for (size_t ii = 0; ii < sprites.size(); ii++) {
		CompoundSprite* sprite = sprites[ii];
		const sf::Vector2f destination = destinations[ii];
		const float maxStepLength = maxStepLengths[ii];
		float angleToDestination;
		float distanceToDestination;

		// move the sprite. Calculate its distance and angle to its destination.
		moveMovableStepTowardsPoint(sprite, destination, maxStepLength, distanceToDestination, angleToDestination);

		// orient components towards destination
		if (distanceToDestination != 0) {
			for (unsigned int indexToRotate : spritesToRotate[ii]) {
				(*sprite->getSprites())[indexToRotate]->setRotation(angleToDestination);
			}
		}

	}
}

/// <summary>
/// Moves a sprite one step forward along path.
/// The sprite will not necessarily reach the end of the path after one call to this function.
/// The sprite will stop after reaching each point in the path even if it is capable of moving farther.
/// </summary>
/// <param name="sprite">The sprite.</param>
/// <param name="path">The path.</param>
/// <param name="msPassed">Time passed in ms.</param>
/// <param name="distPerMs">The maximum distance that the sprite can move per ms.</param>
/// <param name="orientSpriteToDestination">Orients sprites towards their destination if true. Does not orient sprites otherwise.</param>
void GB::moveSpriteAlongPath(sf::Sprite& sprite, 
							 WindowCoordinatePathPtr path,
							 sf::Uint64 msPassed,
							 float distPerMs,
							 const bool orientSpriteToDestination) {
	std::vector<sf::Sprite*> sprites = {&sprite};
	std::vector<WindowCoordinatePathPtr> paths = {path};
	std::vector<float> distPerMsVec = {distPerMs};

	bulkMoveSpriteAlongPath(sprites, paths, msPassed, distPerMsVec, orientSpriteToDestination);
}

/// <summary>
/// Moves all passed sprites one step forward along their respective paths.
/// The sprites will not necessarily reach the end of their paths after one call to this function.
/// The sprites will stop after reaching each point in the path even if they are capable of moving farther.
/// </summary>
/// <param name="sprites">The sprites.</param>
/// <param name="paths">The paths. Each sprite will follow the path with the matching index.</param>
/// <param name="msPassed">Time passed in ms.</param>
/// <param name="distPerMs">The maximum distance that the each sprite can move per ms. Each distance corresponds to the sprite with the matching index.</param>
/// <param name="orientSpritesToDestination">The orient sprites to destination.</param>
void GB::bulkMoveSpriteAlongPath(const std::vector<sf::Sprite*>& sprites, 
								 const std::vector<WindowCoordinatePathPtr>& paths,
								 const sf::Uint64 msPassed, 
								 const std::vector<float>& distPerMs,
								 const bool orientSpritesToDestination) {
	// ensure that the input sizes match
	if (sprites.size() != paths.size() || sprites.size() != distPerMs.size()) {
		throw Error::NavigationTools_MismatchedNavigationSizes();
	}


	// determine speed and destination
	std::vector<float> maxStepLengths;
	maxStepLengths.reserve(sprites.size());
	std::vector<sf::Vector2f> destinations;
	destinations.reserve(sprites.size());
	for (unsigned int ii = 0; ii < paths.size(); ++ii) {
		if (!paths[ii]->empty()) {
			destinations.push_back(paths[ii]->front());
		} else {
			destinations.push_back(sprites[ii]->getPosition());
			paths[ii]->push_back(sprites[ii]->getPosition());
		}
		maxStepLengths.push_back(msPassed*distPerMs[ii]);
	}

	// move the sprites
	bulkMoveSpriteStepTowardsPoint(sprites, destinations, maxStepLengths, orientSpritesToDestination);

	// if the sprite has reached destination, move on to next point in path
	for (unsigned int ii = 0; ii < sprites.size(); ii++) {
		sf::Sprite* sprite = sprites[ii];
		if (sprite->getPosition() == destinations[ii]){
			paths[ii]->pop_front();
		}
	}
}
