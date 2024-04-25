#pragma once
#include "Sound.hpp"
#include "Silence.hpp"
#include "SoundFromFile.hpp"
#include "PeriodicalSound.hpp"
#include "Mix.hpp"
#include "Sequence.hpp"
#include "Effect.hpp"

#include "Normalize.hpp"
#include "ChangeSound.hpp"
#include "FadeOut.hpp"
#include "FadeIn.hpp"

unsigned getSpasesFromLine(std::ifstream& ifs)
{
	if (!ifs.is_open()) {
		throw std::invalid_argument("File is not open.");
	}
	size_t currentPosition = ifs.tellg();
	unsigned int count = 0;
	while (true) {
		char current = ifs.get();
		if (current == '\n' || current == '\0' || ifs.eof())
			break;
		else if (current == ' ')
			count++;
	}
	ifs.clear();
	ifs.seekg(currentPosition);
	return count;
}
unsigned getCharCountFromFile(std::ifstream& ifs, char ch) 
{
	if (!ifs.is_open()) {
		throw std::invalid_argument("File is not open.");
	}
	size_t currentPosition = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	unsigned int count = 0;
	while (true) {
		char current = ifs.get();
		if (ifs.eof())
			break;
		else if (current == ch)
			count++;
	}
	ifs.clear();
	ifs.seekg(currentPosition);
	return count;
}


template <typename T>
class Playlist {
private:
	Sound<T>** sounds;
	unsigned size;
	unsigned capacity;
public:
	Playlist(const char* playlistFileName);

	Playlist(const Playlist<T>& other);
	Playlist<T>& operator=(const Playlist<T>& other);
	~Playlist();

	Sound<T>* operator[](unsigned index);
	const Sound<T>operator[](unsigned index) const;
	Sound<T>* operator()() const;
	void saveInFile(const char* name) const;
private:
	void copyFrom(const Playlist<T>& other);
	void free();
};


template<typename T>
inline Playlist<T>::Playlist(const char* playlistFileName)
{
	std::ifstream ifs(playlistFileName);
	if (!ifs.good()) {
		throw std::invalid_argument("File is not open");
	}
	size = 0;
	capacity = getCharCountFromFile(ifs, '\n') + 1;
	sounds = new Sound<T>*[capacity];
	for (size_t i = 0; i < capacity; i++) {
		unsigned spases = getSpasesFromLine(ifs);
		char buff[1001];
		unsigned number = 0;
		double coef = 0;
		if (spases == 0) {
			ifs >> number;
			ifs.ignore();
			sounds[size++] = new Silence<T>(number);
		}
		if (spases == 1) {
			ifs >> buff;
			ifs >> coef;
			ifs.ignore();
			sounds[size++] = new Effect<T, ChangeSound<T>>(new SoundFromFile<T>(buff), ChangeSound<T>(coef));
			
		}
		if (spases == 2) {
			ifs >> buff;
			ifs >> number >> coef;
			ifs.ignore();
			sounds[size++] = new Effect<T, FadeIn<T>>(new SoundFromFile<T>(buff), FadeIn<T>(number, coef));
		}
	}
	ifs.close();
}

template<typename T>
inline Playlist<T>::Playlist(const Playlist<T>& other)
{
	copyFrom(other);
}

template<typename T>
inline Playlist<T>& Playlist<T>::operator=(const Playlist<T>& other)
{
	if (this != &other) {
		free();
		copyFrom();
	}
	return *this;
}

template<typename T>
inline Playlist<T>::~Playlist()
{
	free();
}

template<typename T>
inline Sound<T>* Playlist<T>::operator[](unsigned index)
{
	if (index >= size) {
		throw std::invalid_argument("Index is bigger than size.");
	}
	return sounds[index];
}

template<typename T>
inline const Sound<T> Playlist<T>::operator[](unsigned index) const
{
	if (index >= size) {
		throw std::invalid_argument("Index is bigger than size.");
	}
	return sounds[index];
}

template<typename T>
inline Sound<T>* Playlist<T>::operator()() const
{
	return new Sequence<T>(sounds, size);
}

template<typename T>
inline void Playlist<T>::saveInFile(const char* name) const
{
	std::ofstream ofs(name, std::ios::binary);
	if (!ofs.good()) {
		throw std::invalid_argument("File is not open.");
	}
	for (size_t i = 0; i < size; i++) {
		sounds[i]->saveInBinaryFile(ofs);
	}
}

template<typename T>
inline void Playlist<T>::copyFrom(const Playlist<T>& other)
{
	size = other.size;
	capacity = other.capacity;
	sounds = new Sound<T>*[capacity];
	for (size_t i = 0; i < capacity; i++) {
		sounds[i] = other.sounds[i]->clone();
	}
}

template<typename T>
inline void Playlist<T>::free()
{
	for (size_t i = 0; i < capacity; i++){
		delete sounds[i];
	}
	delete[] sounds;
}
