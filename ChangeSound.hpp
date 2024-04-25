#pragma once
#include "Effect.hpp"


template<typename T>
class ChangeSound {
private:
	double coef;
	T border;
public:
	ChangeSound() = default;
	ChangeSound(double coef, T border = -1);
	T operator()(T sample, unsigned index) const ;
};

template<typename T>
inline ChangeSound<T>::ChangeSound(double coef, T border) : coef(coef), border(border)
{
}

template<typename T>
inline T ChangeSound<T>::operator()(T sample, unsigned index) const
{
	return sample * coef > border ? border : sample * coef;
}
