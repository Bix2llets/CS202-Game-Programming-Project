// Random.hpp
// A lightweight wrapper around a 64-bit Mersenne Twister PRNG providing
// convenience methods for generating various primitive random values.

#pragma once

#include <random>
#include <limits>
#include <cstdint>

class Random {
public:
	using Engine = std::mt19937_64; // 64-bit engine

	explicit Random(std::uint64_t seed = std::random_device{}())
		: m_engine(seed) {}

	// Reseed the engine.
	void reseed(std::uint64_t seed) { m_engine.seed(seed); }

	// Return a random unsigned 64-bit raw value (full engine output).
	std::uint64_t nextU64() { return m_engine(); }

	// Return a random long long in [0, max].
	long long nextLongLong() { return uniformInt<long long>(std::numeric_limits<long long>::min(), std::numeric_limits<long long>::max()); }

	// Return a random int in [min, max] (defaults to full positive 32-bit range).
	int nextInt(int min = 0, int max = std::numeric_limits<int>::max()) {
		return uniformInt<int>(min, max);
	}

	// Return a random float in [min, max). Defaults to [0,1).
	float nextFloat(float min = 0.0f, float max = 1.0f) {
		return uniformReal<float>(min, max);
	}

	// Return a random double in [min, max). Defaults to [0,1).
	double nextDouble(double min = 0.0, double max = 1.0) {
		return uniformReal<double>(min, max);
	}

	// Generic helpers ----------------------------------------------------
	template <class Int>
	Int uniformInt(Int min, Int max) {
		std::uniform_int_distribution<Int> dist(min, max);
		return dist(m_engine);
	}

	template <class Real>
	Real uniformReal(Real min, Real max) {
		std::uniform_real_distribution<Real> dist(min, max);
		return dist(m_engine);
	}

private:
	Engine m_engine;
};

