#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

template <typename T>
class Sound {
private:
	unsigned sampleLength;
	T minBorder;
public:
	Sound(unsigned sampleLength, T minBorder);
	unsigned getSampleLength() const;
	double getMinBorder() const;
	const char* getType() const;

	virtual T operator[](unsigned index) const = 0;
	virtual Sound* clone() const = 0;
	virtual void saveInBinaryFile(std::ofstream& ofs) const = 0;
	virtual ~Sound() = default;
};	

template<typename T>
inline Sound<T>::Sound(unsigned sampleLength, T minBorder) : sampleLength(sampleLength), minBorder(minBorder > 0 ? ((-1) * minBorder) : minBorder)
{
}
template<typename T>
inline const char* Sound<T>::getType() const
{
	return sampleLength == 0 ? "Pause" : "Song";
}
template<typename T>
inline unsigned Sound<T>::getSampleLength() const
{
	return sampleLength;
}
template<typename T>
inline double Sound<T>::getMinBorder() const
{
	return minBorder;
}