#pragma once
#include <cstdint>
#include <random>
#include <type_traits>
#include <cstdio>
#include <cstdlib>
#include <ctime>

class Randomizer
{
public:

	Randomizer() 
	{

	}

    inline auto nextBool() const -> bool
	{
		return nextInt(2) == 1;
	}

    inline auto below(float percent) const -> bool
	{
		return nextReal(1.0f) < percent;
	}

    inline auto above(float percent) const -> bool
	{
		return nextReal(1.0f) > percent;
	}

	template <class T>
    inline auto nextReal(T to) const -> T
	{
		return nextReal<T>(0, to);
	}

	template <class T>
    inline auto nextReal(T from, T to) const -> T
	{
		if (from >= to)
			return from;
		std::uniform_real_distribution<T> range(from, to);
		std::random_device rd;
		std::default_random_engine engine{ rd() };
		return range(engine);
	}

	template <class T>
    inline auto nextInt(T to) const -> T
	{
		return nextInt<T>(0, to);
	}

	template <class T>
    inline auto nextInt(T from, T to) const -> T
	{
		if (from >= to)
			return from;

		std::uniform_int_distribution<T> range(from, to - 1);
		std::random_device rd;
		std::default_random_engine engine{ rd() };
		return range(engine);

	}

    static auto random() -> float
	{
        return get().nextReal(0.f, 1.f);
	}

    static auto get() -> Randomizer&{
        static Randomizer singleton;
        return singleton;
    }
};
