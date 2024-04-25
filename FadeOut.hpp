#pragma once
#include "Effect.hpp"

template <typename T>
class  FadeOut {
private:
	unsigned samplesLength;
	unsigned coef;
	double minLevel;
	unsigned soundLength;
public:
	FadeOut() = default;
	FadeOut(unsigned samplesLength, double minLevel, unsigned soundLength);
	T operator()(T sample, unsigned index) const;
};

template<typename T>
inline FadeOut<T>::FadeOut(unsigned samplesLength, double minLevel, unsigned soundLength) : samplesLength(samplesLength), minLevel(minLevel),
soundLength(soundLength)
{
}

template<typename T>
inline T FadeOut<T>::operator()(T sample, unsigned index) const
{
	return index < soundLength - samplesLength ? sample : (1.0 / samplesLength) * (soundLength - index - 1) * sample;
}
