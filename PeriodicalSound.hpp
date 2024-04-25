#pragma once
#include "Sound.hpp"

template <typename T>
class PeriodicalSound : public Sound<T> {
private:
	T* samples;
public:
	PeriodicalSound(const T* samples, unsigned size, unsigned repeats, T border = -1);
	PeriodicalSound(const PeriodicalSound<T>& other);
	PeriodicalSound<T>& operator=(const PeriodicalSound<T>& other);
	~PeriodicalSound();

	T operator[](unsigned index) const override;
	Sound<T>* clone() const override;
	void saveInBinaryFile(std::ofstream& ofs) const override;
private:
	void checkValidNumber(unsigned size);
	void free();
	void copyFrom(const PeriodicalSound<T>& other);
};

template<typename T>
inline PeriodicalSound<T>::PeriodicalSound(const T* samples, unsigned size, unsigned repeats, T border) : Sound<T>(size * repeats, border)
{
	this->samples = new (std::nothrow) T[Sound<T>::getSampleLength()];
	if (this->samples == nullptr) {
		throw std::invalid_argument("No allocate memory!");
	}
	for (size_t i = 0; i < size; i++) {
		this->samples[i] = samples[i];
	}
	checkValidNumber(size);
	unsigned index = size;
	for (size_t i = 1; i < repeats; i++) {
		for (size_t j = 0; j < size; j++) {
			this->samples[index++] = this->samples[j];
		}
	}
}

template<typename T>
inline PeriodicalSound<T>::PeriodicalSound(const PeriodicalSound<T>& other) : Sound<T>(other)
{
	copyFrom(other);
}

template<typename T>
inline PeriodicalSound<T>& PeriodicalSound<T>::operator=(const PeriodicalSound<T>& other)
{
	if (this != &other) {
		Sound<T>::operator=(other);
		free();
		copyFrom(other);
	}
	return *this;
}

template<typename T>
inline PeriodicalSound<T>::~PeriodicalSound()
{
	free();
}

template<typename T>
inline T PeriodicalSound<T>::operator[](unsigned index) const
{
	if (index >= Sound<T>::getSampleLength()) {
		throw std::invalid_argument("Index is bigger than size of samples.");
	}
	return samples[index];
}

template<typename T>
inline Sound<T>* PeriodicalSound<T>::clone() const
{
	return new PeriodicalSound(*this);
}

template<typename T>
inline void PeriodicalSound<T>::saveInBinaryFile(std::ofstream& ofs) const
{
	if (!ofs.good()) {
		throw std::invalid_argument("File is not open.");
	}
	ofs.write((const char*)samples, sizeof(T) * Sound<T>::getSampleLength());
}

template<typename T>
inline void PeriodicalSound<T>::checkValidNumber(unsigned size)
{
	double border = Sound<T>::getMinBorder();
	for (size_t i = 0; i < size; i++) {
		if (samples[i] < border) {
			samples[i] = border;
		}
		else if (samples[i] > (-1 * border)) {
			samples[i] = -1 * border;
		}
	}
}

template<typename T>
inline void PeriodicalSound<T>::free()
{
	delete[] samples;
}

template<typename T>
inline void PeriodicalSound<T>::copyFrom(const PeriodicalSound<T>& other)
{
	unsigned size = Sound<T>::getSampleLength();
	samples = new (std::nothrow) T[size];
	if (samples == nullptr) {
		throw std::invalid_argument("No alocate memory!");
	}
	for (size_t i = 0; i < size; i++) {
		samples[i] = other.samples[i];
	}
}
