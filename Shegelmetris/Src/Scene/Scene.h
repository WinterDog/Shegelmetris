#pragma once

#include "../Cell/Cell.h"
#include "../Figure/Figure.h"

namespace wd
{
	class VideoDevice;

	//-----------------------------------------------------------------------------
	// Game scene of the application.
	//-----------------------------------------------------------------------------
	class Scene
	{
	public:
		typedef std::vector<std::vector<
			std::unique_ptr<wd::Cell>>>				CellMatrix;

	private:
		// Size of the entering zone.
		// Default value is 4, so the line in its vertical state may be placed there.
		static const std::size_t					m_enterZoneSize;

		// Save video device pointer for drawing.
		wd::VideoDevice* const						m_pVideoDevice;

		// Size of the game field, excluding the top entering zone.
		const glm::u32vec2							m_fieldSize;

		// Starting game speed.
		double										m_startingSpeed;
		// Current game speed.
		// Figures will fall by 1 row at each (1.0 / m_speed) second.
		double										m_speed;
		// How much seconds to add to the speed with each new figure.
		double										m_speedIncrement;

		// Current score.
		std::size_t									m_score;

		// Rows which were removed cause they were full.
		// All the cells above should be shifted down.
		// Note that the indices of the rows are edded here from bottom to the top,
		// so to properly shift the rows above
		// we should move from the end of the list to the beginning.
		std::list<std::size_t>						m_emptyRows;

		// Matrix of the cells, including the top entering zone.
		CellMatrix									m_cellMatrix;
		// Border cells, forming the game field.
		std::list<wd::Cell>							m_borders;

		// Current figure.
		std::unique_ptr<wd::Figure>					m_pCurFigure;
		// Next figure.
		std::unique_ptr<wd::Figure>					m_pNextFigure;

		// We should check for full rows at the next Update().
		bool										m_checkFullRows;

		// Game is over - show message and stop the game.
		bool										m_gameOver;
		// We are currently waiting for the player to press Enter to start new game.
		bool										m_waitingForStart;

	public:
		Scene(
			const glm::u32vec2& fieldSize,
			wd::VideoDevice* const pVideoDevice);

		~Scene(
			void);

	public:
		// Create new playable figure.
		// Also creates the next figure, if needed.
		void CreateFigure(
			void);

		// Set starting speed of the game.
		void SetStartingSpeed(
			const double& speed);

		// Set speed increment value.
		void SetSpeedIncrement(
			const double& increment);

		// Process scene by one step.
		void StepLogic(
			const double& timeDelta);

		// Draw a single frame of the scene.
		void Draw(
			const double& timeDelta);

	private:
		// Create border cells (fill m_borders).
		void CreateBorders(
			void);

		// Initialize cells with empty pointers.
		void InitCellMatrix(
			void);

		// Draw waiting state stuff.
		void RenderWaitingState(
			void);

		// Draw gameplay state stuff.
		void RenderGameState(
			const double& timeDelta);

		// Stop the game.
		void GameOver(
			void);

		// Restart the game.
		void RestartGame(
			void);

		// Check the enter zone at the top.
		// If there are cells, stop the game.
		void CheckLoseCondition(
			void);

		// Check full rows.
		// If found, remove them and shift the cells above.
		void CheckFullRows(
			void);

		// Kill row with index [row].
		void KillRow(
			const std::size_t& row);

		// Shift top rows to the bottom at the place of removed full rows (m_emptyRows).
		void FillEmptyRows(
			void);
	};
};
