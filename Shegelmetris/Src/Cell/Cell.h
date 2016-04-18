#pragma once

namespace wd
{
	class Scene;

	//-----------------------------------------------------------------------------
	// Single cell.
	//-----------------------------------------------------------------------------
	class Cell
	{
	public:
		// Current cell status.
		enum class Status : std::uint8_t
		{
			// Cell is in its normal state.
			Alive,
			// Cell was killed and now is being animated.
			Dying,
			// Cell finished its animation and should be destroyed.
			Dead,
		};

	private:
		// Color of the dead cell.
		static const glm::vec3			m_deadColor;
		// Time of the dying animation.
		static const double				m_dyingTime;

		// Initial cell color.
		const glm::vec3					m_initialColor;

		// Current cell status.
		Status							m_status;

		// World position.
		glm::vec2						m_position;
		// Current color.
		glm::vec3						m_color;

		// Timer for the dying animation.
		double							m_animationTimer;

	public:
		Cell(
			const glm::vec3& color,
			const glm::vec2& position);

		Cell(
			const glm::vec3& color,
			const glm::i32vec2& position);

		Cell(
			const glm::vec3& color);

		~Cell(
			void);

	public:
		const Status& GetStatus(
			void) const;

		 void SetStatus(
			 const Status& status);

		const glm::vec2& GetPosition(
			void) const;

		void SetPosition(
			const glm::vec2& position);

		void SetPosition(
			const glm::i32vec2& position);

		const glm::vec3& GetColor(
			void) const;

		// Update the cell.
		// Currently only apdates dying cells - their animation and state.
		void Update(
			const double& timeDelta);

		// Mark cell as Dying and begin to animate its destruction.
		void Kill(
			void);

	private:
	};
};
