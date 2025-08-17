// RandomManager.hpp
// Manages categorized Random instances seeded deterministically from a base seed.

#pragma once

#include <vector>
#include <cstdint>
#include <cassert>
#include "Gameplay/Randomness/Random.hpp"

// Extend / modify categories as needed for systems using randomness.
enum class RandomType : std::size_t {
	TerrainGeneration = 0,
	EntityID,
	EntityEffectId,
	EnemyMovement,

	COUNT // keep last: number of categories
};

class RandomManager {
public:
	explicit RandomManager(std::uint64_t seed = std::random_device{}())
		: m_masterSeed(seed) {
		initializeChildren();
	}

	// Reseed the entire manager and all child generators deterministically.
	void reseed(std::uint64_t newSeed) {
		m_masterSeed = newSeed;
		initializeChildren();
	}

	// Access the Random instance for a given category.
	Random & get(RandomType type) {
		auto idx = static_cast<std::size_t>(type);
		assert(idx < m_generators.size());
		return m_generators[idx];
	}

	const Random & get(RandomType type) const {
		auto idx = static_cast<std::size_t>(type);
		assert(idx < m_generators.size());
		return m_generators[idx];
	}

	std::uint64_t masterSeed() const { return m_masterSeed; }

private:
	void initializeChildren() {
		// Use a SplitMix64 style mixer to derive distinct seeds for each sub-generator.
		m_generators.clear();
		m_generators.reserve(static_cast<std::size_t>(RandomType::COUNT));
		std::uint64_t x = m_masterSeed + 0x9E3779B97f4A7C15ull; // golden ratio base
		for (std::size_t i = 0; i < static_cast<std::size_t>(RandomType::COUNT); ++i) {
			x = splitMix64(x + i * 0x9E3779B97f4A7C15ull);
			m_generators.emplace_back(x);
		}
	}

	static std::uint64_t splitMix64(std::uint64_t z) {
		z += 0x9E3779B97f4A7C15ull;
		z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
		z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
		return z ^ (z >> 31);
	}

private:
	std::uint64_t m_masterSeed;
	std::vector<Random> m_generators;
};

