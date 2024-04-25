#pragma once
#include "Effect.hpp"

template <typename T>
class FadeIn {
private:
	unsigned samplesLength;
	double maxLevel;
public:
	FadeIn() = default;
	FadeIn(unsigned samplesLength, double maxLevel);
	T operator()(T sample, unsigned index) const;
};

template<typename T>
inline FadeIn<T>::FadeIn(unsigned samplesLength, double maxLevel) : samplesLength(samplesLength), maxLevel(maxLevel)
{
}

template<typename T>
inline T FadeIn<T>::operator()(T sample, unsigned index) const
{
	return index < samplesLength ? sample * index * (maxLevel / (samplesLength - 1)) : sample * maxLevel ;
}
//123456
//0 2 4 