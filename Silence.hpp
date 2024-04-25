#pragma once
#include "Sound.hpp"
template <typename T>
class Silence : public Sound<T> {
private:
	static T silence;
public:
	Silence();
	Silence(unsigned sampleLength);
	T operator[](unsigned index) const override;
	Sound<T>* clone() const override;
	void saveInBinaryFile(std::ofstream& ofs) const override;
};

template <typename T>
T Silence<T>::silence = 0;

template<typename T>
inline Silence<T>::Silence() : Sound<T>(1, silence)
{
}
template<typename T>
inline Silence<T>::Silence(unsigned sampleLength) : Sound<T>(sampleLength, silence)
{
}
template<typename T>
inline T Silence<T>::operator[](unsigned index) const
{
	return silence;
}

template<typename T>
inline Sound<T>* Silence<T>::clone() const
{
	return new Silence(*this);
}

template<typename T>
inline void Silence<T>::saveInBinaryFile(std::ofstream& ofs) const
{
	if (!ofs.good()) {
		throw std::invalid_argument("File is not open.");
	}
	unsigned size = Sound<T>::getSampleLength();
	for (size_t i = 0; i < size; i++) {
		ofs.write((const char*)&silence, sizeof(silence));
	}
	
}
