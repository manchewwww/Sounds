#pragma once
#include "Sound.hpp"

template <typename T, typename R>
class Effect : public Sound<T> {
private:
	Sound<T>* sound;
	R function;

public:
	Effect(const Sound<T>* sound, R function, T border = -1);
	T operator[](unsigned index) const override;
	Sound<T>* clone() const override;
	void saveInBinaryFile(std::ofstream& ofs) const override;

	Effect(const Effect<T, R>& other);
	Effect<T,R>& operator=(const Effect<T, R>& other);
	~Effect();
private:
	double maxSound();
	void free();
	void copyFrom(const Effect<T, R>& other);
};

template<typename T, typename R>
inline Effect<T, R>::Effect(const Sound<T>* sound, R function, T border) : Sound<T>(sound->getSampleLength(), border), function(function)
{
	if (sound == nullptr) {
		throw std::invalid_argument("Sound must be different than nullptr");
	}
	this->sound = sound->clone();
}

template<typename T, typename R>
inline Sound<T>* Effect<T, R>::clone() const
{
	return new Effect(*this);
}

template<typename T, typename R>
inline void Effect<T, R>::saveInBinaryFile(std::ofstream& ofs) const
{
	if (!ofs.good()) {
		throw std::invalid_argument("File is not open.");
	}
	for (size_t i = 0; i < sound->getSampleLength(); i++) {
		T elem = function((*sound)[i], i);
		ofs.write((const char*)& elem, sizeof(T));
	}/*
	sound->saveInBinaryFile(ofs);*/
}


template<typename T, typename R>
inline T Effect<T, R>::operator[](unsigned index) const
{
	return function((*sound)[index], index);
}

template<typename T, typename R>
inline Effect<T, R>::Effect(const Effect<T, R>& other) : Sound<T>(other)
{
	copyFrom(other);
}

template<typename T, typename R>
inline Effect<T, R>& Effect<T, R>::operator=(const Effect<T, R>& other)
{
	if (this != &other) {
		Sound<T>::operator=(other);
		free();
		copyFrom(other);
	}
	return *this;
}

template<typename T, typename R>
inline Effect<T, R>::~Effect()
{
	free();
}

template<typename T, typename R>
inline double Effect<T, R>::maxSound()
{
	T maxNumber = abs((*sound)[0]);
	for (size_t i = 1; i < Sound<T>::getSampleLength(); i++) {
		if (maxNumber < abs((*sound)[i])) {
			maxNumber = abs((*sound)[i]);
		}
	}
	return ((Sound<T>::getMinBorder() * (-1)) / maxNumber);
}

template<typename T, typename R>
inline void Effect<T, R>::free()
{
	delete sound;
}

template<typename T, typename R>
inline void Effect<T, R>::copyFrom(const Effect<T, R>& other)
{
	sound = other.sound->clone();
	function = other.function;
}
