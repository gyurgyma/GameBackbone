#pragma once

#include <GameBackbone/Core/BackboneBaseExceptions.h>
#include <GameBackbone/Core/BasicGameRegion.h>
#include <GameBackbone/Core/Updatable.h>
#include <GameBackbone/Core/CompoundSprite.h> // for is_updatable
#include <GameBackbone/Util/DllUtil.h>

#include <SFML/Graphics/Drawable.hpp>

#include <memory>
#include <utility>
#include <vector>
#include <functional>

namespace GB {

	/// <summary> Base class meant to be inherited. Controls game logic and actors for a specific time or space in game. </summary>
	class libGameBackbone GameRegion : public BasicGameRegion {
	public:
		/// <summary>shared_ptr to GameRegion</summary>
		using Ptr = std::shared_ptr<GameRegion>;
		
		GameRegion() = default;
		GameRegion(const GameRegion&) = delete;
		GameRegion& operator=(const GameRegion&) = delete;
		GameRegion(GameRegion&&) = default;
		GameRegion& operator=(GameRegion&&) = default;
		virtual ~GameRegion() = default;


		/// <summary>
		/// Add a drawable with a given priority to this GameRegion.
		/// If the drawable already exists, its priority will be updated.
		/// 
		/// This function will throw an std::invalid_argument exception if a nullptr is passed in. 
		/// </summary>
		/// <param name="priority"> The priority of the drawable </param>
		/// <param name="drawablesToRemove"> The drawable that will be added </param>
		template <class Drawable>
		Drawable& addComponent(int priority, Drawable drawableToAdd)
		{
			// Store it in a unique_ptr
			auto uniqueDrawable = std::make_unique<Drawable>(std::move(drawableToAdd));

			// if its updatable add it to updatables
			if constexpr (is_updatable_v<Drawable>)
			{
				m_updatables.push_back(uniqueDrawable.get());
			}

			// add it to drawables
			auto& emplacedDrawable = m_ownedComponents.emplace_back(std::move(uniqueDrawable));

			// return the original value
			return *static_cast<Drawable*>(emplacedDrawable.get());
		}

		// does this need a priority?
		// Perhaps one with a priority as a hint?
		template <class Drawable>
		void removeComponent(const Drawable& drawableToAdd)
		{
			// if its updatable remove it from updatable
			if constexpr (is_updatable_v<Drawable>)
			{
				m_updatables.erase(
					std::remove_if(
						m_updatables.begin(),
						m_updatables.end(),
						[&drawableToAdd](const auto& value)
						{
							return &drawableToAdd == &value;
						}
				));
			}

			m_ownedComponents.erase(
				std::remove_if(
					m_ownedComponents.begin(),
					m_ownedComponents.end(),
					[](auto& ownedComponent)
					{
						return true;
					}
				)
			);
		}

		// Add/Remove/Clear drawables
		void addDrawable(int priority, sf::Drawable* drawableToAdd);
		void addDrawable(int priority, const std::vector<sf::Drawable*>& drawablesToAdd);
		void removeDrawable(sf::Drawable* drawableToRemove);
		void removeDrawable(const std::vector<sf::Drawable*>& drawablesToRemove);
		void clearDrawables();
		void clearDrawables(int priority);

		[[nodiscard]]
		std::size_t getDrawableCount() const noexcept;
		[[nodiscard]]
		std::size_t getDrawableCount(int priority) const noexcept;

		/// <summary>
		/// Implements Updatable::update as a no-op.
		/// </summary>
		/// <param name="elapsedTime"> </param>
		virtual void update(sf::Int64 elapsedTime) override {
			for (Updatable* updatable : m_updatables)
			{
				updatable->update(elapsedTime);
			}
		}

	protected:
		// Drawing
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		std::vector<std::pair<int, std::vector<sf::Drawable*>>> prioritizedDrawables;

		// Doing this so that I dont have to deal with changing the types of all the helpers. I know that this is ignoring priority
		std::vector<std::unique_ptr<sf::Drawable>> m_ownedComponents;
		std::vector<Updatable*> m_updatables;
	};
}
