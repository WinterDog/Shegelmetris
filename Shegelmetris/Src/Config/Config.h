#pragma once

#include "inc/Serialize.h"

namespace wd
{
	//-----------------------------------------------------------------------------
	// Settings container.
	//-----------------------------------------------------------------------------
	struct Config
	{
	public:
		// Margins. 
		float							marginLeft = 5.0f;
		float							marginTop = 2.0f;
		float							marginBottom = 2.0f;
		// Starting fall speed.
		double							startingSpeed = 1.0;
		// How much the speed will increase with every new figure.
		double							speedIncrement = 0.01;
		// Game field size (width and height).
		// Does not include the top 4-cell entering area (where the figure is spawned).
		glm::u32vec2					fieldSize = { 10, 20 };
		// Screen resolution (width and height).
		glm::u32vec2					resolution = { 800, 600 };
		// Fullscreen mode enabled.
		bool							fullscreenMode = false;

	private:
		// Singleton.
		static const Config*			m_pConfig;

	public:
		Config(
			const std::string& fileName);

		~Config(
			void);

	public:
		static const Config* Get(
			void);

	private:
		void LoadFromXML(
			const std::string& fileName);

	private:
		// Serialization stuff.
		friend class cereal::access;

		template <typename Archive>
		void serialize(
			Archive& archive)
		{
			archive & CEREAL_NVP(marginLeft);
			archive & CEREAL_NVP(marginTop);
			archive & CEREAL_NVP(marginBottom);

			archive & CEREAL_NVP(startingSpeed);
			archive & CEREAL_NVP(speedIncrement);
			archive & CEREAL_NVP(fieldSize);

			archive & CEREAL_NVP(resolution);
			archive & CEREAL_NVP(fullscreenMode);
		};
	};
};
