#pragma once

namespace wd
{
	class Cell;

	//-----------------------------------------------------------------------------
	// Figure controlled by a player.
	//-----------------------------------------------------------------------------
	class Figure
	{
	public:
		// 2D cell matrix of the scene, excluding the figure.
		typedef std::vector<std::vector<
			std::unique_ptr<wd::Cell>>>				CellMatrix;

		typedef std::array<
			std::unique_ptr<wd::Cell>, 4>			FigureCells;

		// Form of the figure.
		enum class Type : std::uint8_t
		{
			I,
			O,
			T,
			J,
			L,
			S,
			Z,
		};

	private:
		// Relative positions of the cells in the figure.
		// 4 cells in each figure, so 4 elements in the array.
		// Zero is usually the bottom-center cell.
		typedef std::array<glm::i32vec2, 4>			CellSet;

	private:
		// Predefined colors of the tetramino figures.
		// Note that they should be defined in the same order as the values in Type enum.
		static const std::array<glm::vec3, 7>		m_colors;

		// How much time should pass for the figure to fall by 1 row.
		// As the speed won't change during the figure falling, this is constant.
		const double								m_fallInterval;

		// To delay key events we create timers and delay periods for different keys.
		// As no event system is implemented and we check keys each frame,
		// we simply set the timer after successful key check
		// and skip its checking while the timer is not zeroed.
		static const double							m_keyPressDelay;
		static const double							m_keyPressShiftDelay;
		static const double							m_keyPressDownDelay;

		const Type									m_type;

		// Timer for the fall behavior of the figure.
		// When it reaches 0.0, the figure should fall by 1 row.
		double										m_fallTimer;
		// Key press delay timers for different key types.
		double										m_keyPressDelayTimer;
		double										m_keyPressShiftDelayTimer;
		double										m_keyPressDownDelayTimer;

		// Position of the figure's "control" or zero cell on the cell matrix.
		// Relative positions of the cells will be calculated from this position.
		glm::i32vec2								m_position;

		// 4 cells of the figure.
		// After the figure will be destroyed,
		// its cells will be moved to the cell matrix of the Scene.
		FigureCells									m_cells;

		// Possible sets of the figure.
		// Each figure can be rotated,
		// and here we store possible configurations of the cells for different rotations.
		// For example, for the cube (type 'O') this vector will contain 1 element,
		// as the cube won't change its configuration with shifting.
		// For the line (type 'I') there will be 2 elements -
		// one for the vertical state of the line and another one for the horizontal.
		// Up to 4 states may be stored here (for 'J', 'L' and 'T' types).
		std::vector<CellSet>						m_possibleSets;
		// Index of the current figure's configuration.
		std::size_t									m_curSetIndex;

		// When the figure hits something to the bottom of it, it becomes dead,
		// and we can release its cells.
		bool										m_active;

	public:
		Figure(
			const double& fallInterval);

		~Figure(
			void);

	public:
		const Type& GetType(
			void) const;

		const glm::i32vec2& GetPosition(
			void) const;

		// Set scene position of the figure.
		void SetPosition(
			const glm::i32vec2& position);

		// Check if the figure is still active (didn't hit any obstacles).
		const bool& IsActive(
			void) const;

		// Get figure cells for rendering.
		const FigureCells& GetCells(
			void) const;

		// Update figure.
		void Update(
			const double& timeDelta,
			const CellMatrix& cells);

		// Move cells from the figure to the cell matrix of the scene [cells].
		void ReleaseCells(
			CellMatrix& cells);

	private:
		// Most of the figures may have 2 or 4 different rotation states (e.g. line - "_" or "|").
		// Here we fill m_possibleSets with possible rotation states for the current figure type.
		void FillCellSets(
			void);

		// Create 4 cells.
		void CreateCells(
			void);

		// Sync cell positions with their current rotation and figure position.
		void UpdateCells(
			void);

		// Move figure left.
		// Returns true if succeded, false if not.
		bool MoveLeft(
			const CellMatrix& cells);

		// Move figure right.
		// Returns true if succeded, false if not.
		bool MoveRight(
			const CellMatrix& cells);

		// Move figure down.
		// Returns true if succeded, false if not.
		bool MoveDown(
			const CellMatrix& cells);

		// Rotate the figure clockwise is possible.
		// Returns true if succeded, false if not.
		bool Shift(
			const CellMatrix& cells);

		// Returns true, if [cellSet] overlaps any cell in [cells].
		bool IsOverlapped(
			const CellSet& cellSet,
			const CellMatrix& cells);

		// When the figure meets
		void Deactivate(
			void);

		// Convert local-space cell positions [relCellSet] to scene-space positions
		// by the figure scene position [figurePos].
		static CellSet CellPositionsRelToAbs(
			const CellSet& relCellSet,
			const glm::i32vec2& figurePos);
	};
};
