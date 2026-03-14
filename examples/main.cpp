#include <iostream>
#include "include/MediumAudio.hpp"
#include <Windows.h>
using namespace aZero;

// API EXAMPLE
int main(int argc, char* argv[]) {
	
	Audio::AudioSystem audioSystem;
	Audio::AudioSource source;

	source = audioSystem.LoadAudio("C:/Users/Noah Schierenbeck/Music/test.wav").value();
	source.Play();

	Sleep(10000);

	return 0;
}