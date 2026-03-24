#include <iostream>
#include <chrono>
#include "include/aZeroAudio.hpp"

#ifdef COMPILE_EXAMPLE
#include <Windows.h>
int main(int argc, char* argv[]) {
	
	aZero::Audio::AudioEngine audioEngine;
	aZero::Audio::AudioSource source = audioEngine.CreateSourceFromFile(std::string(AUDIO_SOURCE_PATH) + "drums.wav").value();

	using clock = std::chrono::high_resolution_clock;
	auto lastTime = clock::now();
	float angle = 0.f;
	float speed = 1500.f;
	float radius = 5.f;
	audioEngine.SetListenerForward(0, 0, 1);
	audioEngine.SetListenerPosition(0, 0, 0.f);
	source.Play();
	source.Fade(5000, 0, 1);
	while (source.IsPlaying()) {
		auto currentTime = clock::now();
		std::chrono::duration<float> delta = currentTime - lastTime;
		lastTime = currentTime;
		float deltaTime = delta.count();

		float x = radius * cos(angle);
		float y = radius * sin(angle);
		static bool idk = true;
		if (angle > 10000 && idk) {
			idk = false;
			//source.Seek(0);
			//source.Fade(5000, -1, 0);
		}
		std::cout << angle << "\n";
		source.SetWorldPosition(x, y, 0);
		angle += speed * deltaTime;
	}

	return 0;
}
#endif