#pragma once
#include <memory>
#include "miniaudio.h"

/* TODO:
	- Check so ::SetRange() is correct
	- Implement support for AudioSource mixing/groups
	- Wrap the following: 
						ma_sound_set_cone
						ma_sound_set_attenuation_model
						ma_sound_set_rolloff
						ma_sound_set_min_gain
						ma_sound_set_max_gain
						ma_sound_set_doppler_factor
*/

namespace aZero::Audio {
	class AudioSource {
	public:
		AudioSource(const AudioSource&) = delete;
		AudioSource& operator=(const AudioSource&) = delete;

		AudioSource() = default;
		AudioSource(ma_engine* engine)
			:m_Engine(engine) { }

		AudioSource(ma_engine* engine, std::string_view path, bool shouldStream) 
			:m_Engine(engine) {
			this->FromFile(path, shouldStream);
		}

		~AudioSource() {
			this->Free();
		}

		AudioSource(AudioSource&& other) noexcept {
			*this = std::move(other);
		}

		AudioSource& operator=(AudioSource&& other) noexcept {
			std::swap(m_Sound, other.m_Sound);
			std::swap(m_Engine, other.m_Engine);
			std::swap(m_ShouldStream, other.m_ShouldStream);
			return *this;
		}

		void Play() {
			ma_sound_start(m_Sound.get());
		}

		void Stop(bool shouldRewind = true) {
			if (shouldRewind) {
				this->Seek(0);
			}
			ma_sound_stop(m_Sound.get());
		}

		// -1 is the current volume
		void Fade(uint64_t msTime, float volumeStart, float volumeEnd) {
			ma_sound_set_fade_in_milliseconds(m_Sound.get(), volumeStart, volumeEnd, static_cast<ma_uint64>(msTime));
		}

		void Seek(uint64_t frame) {
			ma_sound_seek_to_pcm_frame(m_Sound.get(), frame);
		}

		void SetSpatialization(bool enabled) {
			ma_sound_set_spatialization_enabled(m_Sound.get(), enabled);
			ma_sound_set_positioning(m_Sound.get(), enabled ? ma_positioning_relative : ma_positioning_absolute);
		}

		void SetVolume(float volume) {
			ma_sound_set_volume(m_Sound.get(), volume);
		}

		void SetLooping(bool shouldLoop) {
			ma_sound_set_looping(m_Sound.get(), static_cast<ma_bool32>(shouldLoop));
		}

		void SetWorldPosition(float x, float y, float z) {
			ma_sound_set_position(m_Sound.get(), x, y, z);
		}

		void SetRange(float range) {
			ma_sound_set_min_distance(m_Sound.get(), 0);
			ma_sound_set_max_distance(m_Sound.get(), range);
		}

		void SetPan(float pan) {
			ma_sound_set_pan(m_Sound.get(), pan);
		}

		void SetPitch(float pitch) {
			ma_sound_set_pitch(m_Sound.get(), pitch);
		}

		void SetVelocity(float x, float y, float z) {
			ma_sound_set_velocity(m_Sound.get(), x, y, z);
		}

		std::optional<AudioSource> CreateInstance() {
			if (m_ShouldStream) {
				return {};
			}
			AudioSource source(m_Engine);
			source.m_Sound = std::make_unique<ma_sound>();
			ma_sound_init_copy(m_Engine, m_Sound.get(), 0, nullptr, source.m_Sound.get());
			return source;
		}

		bool Load(std::string_view path, bool shouldStream) {
			if (!m_Engine) return false;
			return this->FromFile(path, shouldStream);
		}

		bool IsPlaying() const {
			return ma_sound_is_playing(m_Sound.get());
		}

	private:
		void Free() {
			if (m_Sound) {
				ma_sound_uninit(m_Sound.get());
			}
		}

		bool FromFile(std::string_view path, bool shouldStream) {
			ma_result result;

			this->Free();

			m_Sound = std::make_unique<ma_sound>();

			m_ShouldStream = shouldStream;
			
			result = ma_sound_init_from_file(m_Engine, path.data(), m_ShouldStream ? ma_sound_flags::MA_SOUND_FLAG_STREAM : NULL, NULL, NULL, m_Sound.get());
			if (result != MA_SUCCESS) {
				return false;
			}
			return true;
		}

		std::unique_ptr<ma_sound> m_Sound;
		ma_engine* m_Engine = nullptr;
		bool m_ShouldStream = false;
	};
}