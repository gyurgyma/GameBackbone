#include <GameBackbone/Core/BasicGameRegion.h>

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <exception>
#include <utility>

using namespace GB;

/// <summary>
/// Initializes a new instance of the <see cref="BasicGameRegion"/> class.
/// </summary>
BasicGameRegion::BasicGameRegion() : m_nextRegion(*this) {}

/// <summary>
/// Gets the game region that should become active after the next update of this one.
/// </summary>
/// <returns>The region that should become active after the next update of this one.</returns>
BasicGameRegion& BasicGameRegion::getNextRegion()
{
	return m_nextRegion;
}

/// <summary>
/// The game region that should become active after the next update of this one.
/// </summary>
/// <returns>The region that should become active after the next update of this one.</returns>
const BasicGameRegion& BasicGameRegion::getNextRegion() const
{
	return m_nextRegion;
}

/// <summary>
/// Sets the BasicGameRegion that should become active after the next update of this one.
/// </summary>
/// <param name="nextRegion">The region that should become active after the next update of this one.</param>
void BasicGameRegion::setNextRegion(BasicGameRegion& nextRegion)
{
	m_nextRegion = nextRegion;
}
