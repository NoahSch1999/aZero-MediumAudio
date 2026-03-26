#pragma once
#include <stdexcept>
#include <optional>
#include "AudioSource.hpp"

/* TODO:
	- Wrap ma_engine_listener_set_cone
	- Implement support for multiple listeners
*/

namespace aZero::Audio {
	class AudioEngine {
	public:
		AudioEngine(const AudioEngine&) = delete;
		AudioEngine& operator=(const AudioEngine&) = delete;

		AudioEngine() 
			:m_Engine(std::make_unique<ma_engine>()) {
			ma_result result = ma_engine_init(NULL, m_Engine.get());
			if (result != MA_SUCCESS) {
				throw std::runtime_error("Failed to create audio engine");
			}
		}

		AudioEngine(AudioEngine&& other) noexcept {
			*this = std::move(other);
		}

		AudioEngine& operator=(AudioEngine&& other) noexcept {
			std::swap(m_Engine, other.m_Engine);
			return *this;
		}

		~AudioEngine() {
			if (m_Engine) {
				ma_engine_uninit(m_Engine.get());
			}
		}

		std::optional<AudioSource> CreateFromFile(std::string_view path, bool shouldStream) {
			AudioSource source(m_Engine.get());
			if (source.Load(path, shouldStream))
				return source;
			return {};
		}

		AudioSource CreateEmpty() {
			return AudioSource(m_Engine.get());
		}

		void SetMasterVolume(float volume) {
			ma_engine_set_volume(m_Engine.get(), volume);
		}

		void SetListenerPosition(float x, float y, float z) {
			ma_engine_listener_set_position(m_Engine.get(), 0, x, y, z);
		}

		void SetListenerForward(float x, float y, float z) {
			ma_engine_listener_set_direction(m_Engine.get(), 0, x, y, z);
		}

		void SetListenerUp(float x, float y, float z) {
			ma_engine_listener_set_world_up(m_Engine.get(), 0, x, y, z);
		}

	private:
		std::unique_ptr<ma_engine> m_Engine;
	};
}