#pragma once
#include "Sound.hpp"

template <typename T>
class Mix : public Sound<T> {
private:
	T* samples;
public:
	Mix(const Sound<T>*const* sounds, unsigned size, T border = -1);
	Mix(const Mix<T>& other);
	Mix<T>& operator=(const Mix<T>& other);
	~Mix();

	T operator[](unsigned index) const override;
	Sound<T>* clone() const override;
	void saveInBinaryFile(std::ofstream& ofs) const override;
private:
	void checkValidNumber();
	unsigned getMaxSampleLenght(const Sound<T>*const* sounds, unsigned size);
	void free();
	void copyFrom(const Mix<T>& other);
};

template<typename T>
inline Mix<T>::Mix(const Sound<T>* const* sounds, unsigned size, T border) : Sound<T>(getMaxSampleLenght(sounds, size), border)
{
	int maxSize = Sound<T>::getSampleLength();
	samples = new (std::nothrow) T[maxSize];
	if (samples == nullptr) {
		throw std::invalid_argument("No allocate memory");
	}
	T sum = 0;
	for (size_t i = 0; i < maxSize; i++) {
		for (size_t j = 0; j < size; j++) {
			try {
				sum += sounds[j]->operator[](i);
			} catch (const std::invalid_argument& e) {
				sum += 0;
			}
		}
		samples[i] = sum / size;
		sum = 0;
	}
	checkValidNumber();
}

template<typename T>
inline Mix<T>::Mix(const Mix<T>& other) : Sound<T>(other)
{
	copyFrom(other);
}

template<typename T>
inline Mix<T>& Mix<T>::operator=(const Mix<T>& other)
{
	if (this != &other) {
		Sound<T>::operator=(other);
		free();
		copyFrom(other);
	}
	return *this;
}

template<typename T>
inline Mix<T>::~Mix()
{
	free();
}

template<typename T>
inline T Mix<T>::operator[](unsigned index) const
{
	if (index >= Sound<T>::getSampleLength()) {
		throw std::invalid_argument("Index is bigger than size of samples");
	}
	return samples[index];
}

template<typename T>
inline Sound<T>* Mix<T>::clone() const
{
	return new Mix(*this);
}

template<typename T>
inline void Mix<T>::saveInBinaryFile(std::ofstream& ofs) const
{
	if (!ofs.good()) {
		throw std::invalid_argument("File is not open.");
	}
	ofs.write((const char*)samples, sizeof(T) * Sound<T>::getSampleLength());
}

template<typename T>
inline void Mix<T>::checkValidNumber()
{
	double border = Sound<T>::getMinBorder();
	unsigned size = Sound<T>::getSampleLength();
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
inline unsigned Mix<T>::getMaxSampleLenght(const Sound<T>* const* sounds, unsigned size)
{
	unsigned maxSize = sounds[0]->getSampleLength();
	for (size_t i = 1; i < size; i++) {
		if (maxSize < sounds[i]->getSampleLength()) {
			maxSize = sounds[i]->getSampleLength();
		}
	}
	return maxSize;
}

template<typename T>
inline void Mix<T>::free()
{
	delete[] samples;
}

template<typename T>
inline void Mix<T>::copyFrom(const Mix<T>& other)
{
	unsigned size = Sound<T>::getSampleLength();
	samples = new (std::nothrow) T[size];
	if (samples == nullptr) {
		throw std::invalid_argument("No allocate memory.");
	}
	for (size_t i = 0; i < size; i++) {
		samples[i] = other.samples[i];
	}
}