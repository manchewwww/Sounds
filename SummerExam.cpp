#include <iostream>
#include "Playlist.hpp"
int main()
{
	try
	{
		std::ofstream file("file.dat", std::ios::binary);
		double arr[] = { 6,2,0,-2,-2,4,6,8,8 };
		file.write((const char*)arr, 9 * sizeof(double));
		file.close();
		Silence<double> silence(9);
		for (size_t i = 0; i < 9; i++)
		{
			std::cout << silence[i] << " ";
		}
		std::cout << std::endl;
		SoundFromFile<double> soundFromFile("file.dat", 10);
		for (size_t i = 0; i < 9; i++)
		{
			std::cout << soundFromFile[i] << " ";
		}
		std::cout << std::endl;
		double array1[] = { 1,12,-15 };
		PeriodicalSound<double> periodicalSound(array1, 3, 2);
		for (size_t i = 0; i < 6; i++)
		{
			std::cout << periodicalSound[i] << " ";
		}
		std::cout << std::endl;

		Sound<double>** sounds = new Sound<double>*[3];
		sounds[0] = &soundFromFile;
		sounds[1] = &periodicalSound;
		sounds[2] = &silence;
		Mix<double> mix(sounds, 3, 5);
		for (size_t i = 0; i < 9; i++)
		{
			std::cout << mix[i] << " ";
		}
		std::cout << std::endl;
		Sequence<double> sequence(sounds, 3);
		for (size_t i = 0; i < 18; i++)
		{
			std::cout << sequence[i] << " ";
		}
		std::cout << std::endl;

		Normalize<double> normalize(&soundFromFile);
		Effect<double, Normalize<double>> effect(&soundFromFile, normalize, 9);
		for (size_t i = 0; i < 9; i++)
		{
			std::cout << effect[i] << " ";
		}
		std::cout << std::endl;
		FadeIn<double> fadeIn(3, 2);
		Effect<double, FadeIn<double>> effectt(&effect, fadeIn, 9);
		for (size_t i = 0; i < 9; i++)
		{
			std::cout << effectt[i] << " ";
		}
		std::cout << std::endl;
		FadeOut<double> fadeOut(4, 0, 9);
		Effect<double, FadeOut<double>> effects(&soundFromFile, fadeOut);
		for (size_t i = 0; i < 9; i++)
		{
			std::cout << effects[i] << " ";
		}
		delete[] sounds;
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what();
	}
	std::cout << std::endl;
	try
	{
		Playlist<double> playlist("file.txt");
		Sound<double>* ptrSound = playlist[0];
		unsigned size = ptrSound->getSampleLength();
		for (size_t i = 0; i < size; i++) {
			std::cout << (*ptrSound)[i] << " ";
		}
		std::cout << std::endl;
		Sound<double>* ptrSound1 = playlist[1];
		unsigned size1 = ptrSound1->getSampleLength();
		for (size_t i = 0; i < size1; i++) {
			std::cout << (*ptrSound1)[i] << " ";
		}
		std::cout << std::endl;
		Sound<double>* ptrSound2 = playlist[2];
		unsigned size2 = ptrSound2->getSampleLength();
		for (size_t i = 0; i < size2; i++) {
			std::cout << (*ptrSound2)[i] << " ";
		}
		std::cout << std::endl;
		playlist.saveInFile("file1.dat");
		SoundFromFile<double> sff("file1.dat");
		for (size_t i = 0; i < sff.getSampleLength(); i++)
		{
			std::cout << sff[i] << " ";
		}
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what() << std::endl;
	}
}