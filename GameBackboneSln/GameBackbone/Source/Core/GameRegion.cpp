#include <GameBackbone/Core/GameRegion.h>

#include <GameBackbone/Core/BasicGameRegion.h>

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <exception>
#include <utility>

using namespace GB;

namespace {

	/// <summary>
	/// Functor used to check if the priority of a priorityPair the same as a given Priority.
	/// </summary>
	class priorityFindComparitor {
	public:
		explicit priorityFindComparitor(int priority) : priorityToFind(priority) {};

		bool operator ()(const std::pair<int, std::vector<sf::Drawable*>>& priorityPair) {
			return priorityPair.first == priorityToFind;
		}

	private:
		int priorityToFind;
	};

	/// <summary>
	/// Functor used to check if the priority of a priorityPair is less than another priorityPair's.
	/// </summary>
	class prioritySortComparitor {
	public:
		bool operator ()(const std::pair<int, std::vector<sf::Drawable*>>& leftEl, const std::pair<int, std::vector<sf::Drawable*>>& rightEl) {
			return leftEl.first < rightEl.first;
		}
	};

	/// <summary>
	/// Functor used to check if a drawable is contained withing a given list of drawables.
	/// </summary>
	class drawablesRemoveComparitor {
	public:
		explicit drawablesRemoveComparitor(const std::vector<sf::Drawable*>& drawables) : drawablesToRemove(drawables) {};

		bool operator ()(const sf::Drawable* drawable) {
			for (auto& drawableToRemove : drawablesToRemove) {
				if (drawable == drawableToRemove) {
					return true;
				}
			}
			return false;
		}

	private:
		const std::vector<sf::Drawable*>& drawablesToRemove;
	};

	/// <summary>
	/// Inserts an element into an std::vector, given a sorting predicate, in the sorted location.
	/// This function assumes that the std::vector is already sorted.
	/// </summary>
	/// <param name="vector"> The vector to add to </param>
	/// <param name="item"> The item that will be added to the vector </param>
	/// <param name="pred"> The predicate with which to find the location of the item </param>
	template< typename T, typename Pred >
	typename std::vector<T>::iterator insert_sorted(std::vector<T> & vector, T const& item, Pred pred)
	{
		return vector.insert(std::upper_bound(vector.begin(), vector.end(), item, pred), item);
	}
}

/// <summary>
/// Add a drawable with a given priority to this GameRegion.
/// If the drawable already exists, its priority will be updated.
/// 
/// This function will throw an std::invalid_argument exception if a nullptr is passed in. 
/// </summary>
/// <param name="priority"> The priority of the drawable </param>
/// <param name="drawablesToRemove"> The drawable that will be added </param>
void GameRegion::addDrawable(int priority, sf::Drawable* drawableToAdd) {
	addDrawable(priority, std::vector<sf::Drawable*>{drawableToAdd});
}

/// <summary>
/// Add drawables with a given priority to this GameRegion.
/// If the drawables already exist, its priority will be updated.
/// 
/// This function will throw an std::invalid_argument exception if a nullptr is passed in. 
/// </summary>
/// <param name="priority"> The priority of the drawables </param>
/// <param name="drawablesToRemove"> The drawables that will be added </param>
void GameRegion::addDrawable(int priority, const std::vector<sf::Drawable*>& drawablesToAdd) {
	// Null check each of the drawables passed in
	for (sf::Drawable* drawable : drawablesToAdd)
	{
		if (drawable == nullptr) {
			throw std::invalid_argument("Cannot invoke GameRegion::addDrawable with a drawable equal to nullptr");
		}
	}

	// Remove any instances of the drawables before adding them again
	removeDrawable(drawablesToAdd);

	auto it = std::find_if(prioritizedDrawables.begin(), prioritizedDrawables.end(), priorityFindComparitor(priority));

	// If the iterator is not end, then we found a pair with the same priority.
	// Insert the drawable at the same priority
	if (it != prioritizedDrawables.end()) {
		std::vector<sf::Drawable*>& tempDrawables = it->second;
		tempDrawables.insert(tempDrawables.end(), drawablesToAdd.begin(), drawablesToAdd.end());
	}
	else {
		insert_sorted(prioritizedDrawables, std::make_pair(priority, drawablesToAdd), prioritySortComparitor());
	}
}

/// <summary>
/// Remove a drawable from this GameRegion. 
/// If the drawable is not found, nothing will be done.
/// </summary>
/// <param name="drawablesToRemove"> The drawable that will be removed </param>
void GameRegion::removeDrawable(sf::Drawable* drawableToRemove) {
	removeDrawable(std::vector<sf::Drawable*>{drawableToRemove});
}

/// <summary>
/// Remove drawables from this GameRegion. 
/// If a drawable is not found, nothing will be done.
/// </summary>
/// <param name="drawablesToRemove"> The drawables that will be removed </param>
void GameRegion::removeDrawable(const std::vector<sf::Drawable*>& drawablesToRemove) {
	// Go through every pair and remove any of the passed in drawables from their vector
	for (auto& priorityPair : prioritizedDrawables) {
		std::vector<sf::Drawable*>& tempDrawables = priorityPair.second;
		auto it = std::remove_if(tempDrawables.begin(), tempDrawables.end(), drawablesRemoveComparitor(drawablesToRemove));
		if (it != tempDrawables.end())
		{
			tempDrawables.erase(it, tempDrawables.end());
		}
	}
}

/// <summary>
/// Removes all drawable objects from this GameRegion.
/// </summary>
void GameRegion::clearDrawables() {
	prioritizedDrawables.clear();
}

/// <summary>
/// Removes all drawable objects with a given priority from this GameRegion.
/// </summary>
/// <param name="priority"> The priority of drawables clear</param>
void GameRegion::clearDrawables(int priority) {
	auto it = std::find_if(prioritizedDrawables.begin(), prioritizedDrawables.end(), priorityFindComparitor(priority));

	// If the iterator is not end, then we found a pair with the same priority.
	// Clear the internal vector
	if (it != prioritizedDrawables.end()) {
		std::vector<sf::Drawable*>& tempDrawables = it->second;
		tempDrawables.clear();
	}
}

/// <summary>
/// Returns the count of all drawables stored on this GameRegion.
/// </summary>
/// <return> The number of drawables </param>
std::size_t GameRegion::getDrawableCount() const  noexcept {
	std::size_t count = 0;
	// Loop through each priority of the drawables
	// Add all of the sizes to the count
	for (const auto& priorityPair : prioritizedDrawables) {
		count += priorityPair.second.size();
	}
	return count;
}

/// <summary>
/// Returns the count of all drawables with a given priority stored on this GameRegion.
/// </summary>
/// <param name="priority"> The priority of drawables to count </param>
/// <return> The number of drawables </param>
std::size_t GameRegion::getDrawableCount(int priority) const noexcept {
	std::size_t count = 0;
	auto it = std::find_if(prioritizedDrawables.begin(), prioritizedDrawables.end(), priorityFindComparitor(priority));

	// If the iterator is not end, then we found a pair with the same priority.
	// Set the count equal to the vectors size
	if (it != prioritizedDrawables.end()) {
		const std::vector<sf::Drawable*>& tempDrawables = it->second;
		count = tempDrawables.size();
	}
	return count;
}

/// <summary>
/// Draws every drawable on the region.
/// </summary>
/// <param name="target"> The SFML render target to draw on. </param>
/// <param name="states"> Current render states </param>
void GameRegion::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Loop through each priority of the drawables
	for (const auto& priorityPair : prioritizedDrawables) {
		for (const sf::Drawable* drawable : priorityPair.second) {
			// Draw each drawable stored in the vector
			target.draw(*drawable, states);
		}
	}
}
