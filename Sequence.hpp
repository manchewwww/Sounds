#pragma once
#include "Sound.hpp"

template <typename T>
class Sequence : public Sound<T> {
private:
	T* samples;
public:
	Sequence(const Sound<T>* const* samples, unsigned size, T border = -1);
	Sequence(const Sequence<T>& other);
	Sequence<T>& operator=(const Sequence<T>& other);
	~Sequence();

	T operator[](unsigned index) const override;
	Sound<T>* clone() const override;
	void saveInBinaryFile(std::ofstream& ofs) const override;
private:
	unsigned sumOfSamplesLength(const Sound<T>* const* samples, unsigned size);
	void free();
	void copyFrom(const Sequence<T>& other);
};

template<typename T>
inline Sequence<T>::Sequence(const Sound<T>* const* samples, unsigned size, T border) : Sound<T>(sumOfSamplesLength(samples, size), border)
{
	int maxSize = Sound<T>::getSampleLength();
	this->samples = new (std::nothrow) T[maxSize];
	if (this->samples == nullptr) {
		throw std::invalid_argument("No allocate memory");
	}
	unsigned index = 0;
	for (size_t i = 0; i < size; i++) {
		unsigned currentSoundLength = samples[i]->getSampleLength();
		for (size_t j = 0; j < currentSoundLength; j++) {
			this->samples[index++] = samples[i]->operator[](j);
		}
	}
}

template<typename T>
inline Sequence<T>::Sequence(const Sequence<T>& other) : Sound<T>(other)
{
	copyFrom(other);
}

template<typename T>
inline Sequence<T>& Sequence<T>::operator=(const Sequence<T>& other)
{
	if (this != &other) {
		Sound<T>::operator=(other);
		free();
		copyFrom(other);
	}
	return *this;
}

template<typename T>
inline Sequence<T>::~Sequence()
{
	free();
}

template<typename T>
inline T Sequence<T>::operator[](unsigned index) const
{
	if (index >= Sound<T>::getSampleLength()) {
		throw std::invalid_argument("Index is bigger than size of samples");
	}
	return samples[index];
}

template<typename T>
inline Sound<T>* Sequence<T>::clone() const
{
	return new Sequence(*this);
}

template<typename T>
inline void Sequence<T>::saveInBinaryFile(std::ofstream& ofs) const
{
	if (!ofs.good()) {
		throw std::invalid_argument("File is not open.");
	}
	ofs.write((const char*)samples, sizeof(T) * Sound<T>::getSampleLength());
}

template<typename T>
inline unsigned Sequence<T>::sumOfSamplesLength(const Sound<T>* const* samples, unsigned size)
{
	unsigned sum = 0;
	for (size_t i = 0; i < size; i++) {
		sum += samples[i]->getSampleLength();
	}
	return sum;
}

template<typename T>
inline void Sequence<T>::free()
{
	delete[] samples;
}

template<typename T>
inline void Sequence<T>::copyFrom(const Sequence<T>& other)
{
	unsigned size = Sound<T>::getSampleLength();
	samples = new (std::nothrow) T[size];
	if (samples == nullptr) {
		throw std::invalid_argument("No allocate memory");
	}
	for (size_t i = 0; i < size; i++) {
		samples[i] = other.samples[i];
	}
}
