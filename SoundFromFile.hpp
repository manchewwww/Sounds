#pragma once
#include "Sound.hpp"

template <typename T>
class SoundFromFile : public Sound<T> {
private:
	T* samples;
public:
	SoundFromFile(const char* fileName, T border = -1);
	SoundFromFile(const SoundFromFile& other);
	SoundFromFile<T>& operator=(const SoundFromFile& other);
	~SoundFromFile();

	T operator[](unsigned index) const override;
	Sound<T>* clone() const override;
	void saveInBinaryFile(std::ofstream& ofs) const override;
private:
	void checkValidNumber();
	unsigned lengthOfSamples(const char* fileName) const;
	void free();
	void copyFrom(const SoundFromFile& other);
};

template<typename T>
inline SoundFromFile<T>::SoundFromFile(const char* fileName, T border) : Sound<T>((lengthOfSamples(fileName) / sizeof(T)), border)
{
	unsigned size = Sound<T>::getSampleLength();
	samples = new (std::nothrow) T[size];
	if (samples == nullptr) {
		throw std::invalid_argument("No alocate memory!");
	}
	std::ifstream file(fileName, std::ios::binary);
	if (!file.good()) {
		throw std::invalid_argument("File is not open.");
	}
	file.read((char*)samples, sizeof(T) * size);
	checkValidNumber();
	file.close();
}
template<typename T>
inline SoundFromFile<T>::SoundFromFile(const SoundFromFile& other) : Sound<T>(other)
{
	copyFrom(other);
}
template<typename T>
inline SoundFromFile<T>& SoundFromFile<T>::operator=(const SoundFromFile& other)
{
	if (this != &other) {
		Sound<T>::operator=(other);
		free();
		copyFrom(other);
	}
	return *this;
}
template<typename T>
inline SoundFromFile<T>::~SoundFromFile()
{
	free();
}
template<typename T>
inline T SoundFromFile<T>::operator[](unsigned index) const
{
	if (index >= Sound<T>::getSampleLength()) {
		throw std::invalid_argument("Index is bigger than size of samples.");
	}
	return samples[index];
}
template<typename T>
inline Sound<T>* SoundFromFile<T>::clone() const
{
	return new SoundFromFile(*this);
}
template<typename T>
inline void SoundFromFile<T>::saveInBinaryFile(std::ofstream& ofs) const
{
	if (!ofs.good()) {
		throw std::invalid_argument("File is not open.");
	}
	ofs.write((const char*)samples, sizeof(T) * Sound<T>::getSampleLength());
}

template<typename T>
inline void SoundFromFile<T>::checkValidNumber()
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
inline unsigned SoundFromFile<T>::lengthOfSamples(const char* fileName) const
{
	std::ifstream file(fileName, std::ios::binary);
	if (!file.good()) {
		throw std::invalid_argument("File is not open.");
	}
	file.seekg(0, std::ios::end);
	unsigned fileSize = file.tellg();
	file.close();
	return fileSize;
}

template<typename T>
inline void SoundFromFile<T>::free()
{
	delete[] samples;
}

template<typename T>
inline void SoundFromFile<T>::copyFrom(const SoundFromFile& other)
{
	unsigned size = other.getSampleLength();
	samples = new (std::nothrow) T[size];
	if (samples == nullptr) {
		throw std::invalid_argument("No alocate memory!");
	}
	for (unsigned i = 0; i < size; i++) {
		samples[i] = other.samples[i];
	}
}

