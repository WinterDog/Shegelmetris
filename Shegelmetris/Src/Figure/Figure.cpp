#include "inc/stdafx.h"

#include "Figure.h"

#include "Cell/Cell.h"
#include "Input/Input.h"
#include "Scene/Scene.h"

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------

// NOTE: Hardcoded number of types.
const std::array<glm::vec3, 7> wd::Figure::m_colors =
{
	glm::vec3(0.0f, 0.941f, 0.941f),
	glm::vec3(0.941f, 0.941f, 0.0f),
	glm::vec3(0.627f, 0.0f, 0.941f),
	glm::vec3(0.0f, 0.0f, 0.941f),
	glm::vec3(0.941f, 0.627f, 0.0f),
	glm::vec3(0.0f, 0.941f, 0.0f),
	glm::vec3(0.941f, 0.0f, 0.0f),
};

// Delay for moving left and right.
const double wd::Figure::m_keyPressDelay = 0.07;
// Rotate key delay.
const double wd::Figure::m_keyPressShiftDelay = 0.12;
// Down key delay.
const double wd::Figure::m_keyPressDownDelay = 0.02;

//-----------------------------------------------------------------------------
// Constructors and destructors
//-----------------------------------------------------------------------------

wd::Figure::Figure(
	const double& fallInterval)
	:
	// Params.
	m_fallInterval(fallInterval),
	m_fallTimer(fallInterval),
	// Defaults.
	// Randomly define and save the type of the figure.
	// NOTE: Hardcoded number of types.
	m_type(static_cast<wd::Figure::Type>(glm::linearRand(0, 6))),
	m_keyPressDelayTimer(0.0),
	m_keyPressShiftDelayTimer(0.0),
	m_keyPressDownDelayTimer(0.0),
	m_active(true)
{
	// Fill rotation cell sets for the current figure type.
	FillCellSets();
	// Randomly define and set the rotation state of the figure.
	m_curSetIndex = glm::linearRand<std::size_t>(0, m_possibleSets.size() - 1);

	// Create cells.
	CreateCells();
	// Sync cells with the figure rotation state and position.
	UpdateCells();
}

wd::Figure::~Figure(
	void)
{
}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

const wd::Figure::Type& wd::Figure::GetType(
	void) const
{
	return m_type;
}

const glm::i32vec2& wd::Figure::GetPosition(
	void) const
{
	return m_position;
}

void wd::Figure::SetPosition(
	const glm::i32vec2& position)
{
	m_position = position;

	UpdateCells();
}

const bool& wd::Figure::IsActive(
	void) const
{
	return m_active;
}

const wd::Figure::FigureCells& wd::Figure::GetCells(
	void) const
{
	return m_cells;
}

void wd::Figure::Update(
	const double& timeDelta,
	const CellMatrix& cells)
{
	// Decrease key delay timers.
	m_keyPressDelayTimer -= timeDelta;
	m_keyPressShiftDelayTimer -= timeDelta;
	m_keyPressDownDelayTimer -= timeDelta;

	// Shift (rotate) key check.
	if (m_keyPressShiftDelayTimer <= 0.0)
	{
		if (wd::Input::IsKeyDown(wd::Input::Key::Action))
		{
			Shift(cells);
			m_keyPressShiftDelayTimer = m_keyPressShiftDelay;
		}
		else
		{
			m_keyPressShiftDelayTimer = 0.0;
		}
	}

	// Left-right key check.
	if (m_keyPressDelayTimer <= 0.0)
	{
		bool keyPressed = false;

		if (wd::Input::IsKeyDown(wd::Input::Key::Left))
		{
			MoveLeft(cells);
			keyPressed = true;
		}
		if (wd::Input::IsKeyDown(wd::Input::Key::Right))
		{
			MoveRight(cells);
			keyPressed = true;
		}

		if (keyPressed)
			m_keyPressDelayTimer = m_keyPressDelay;
		else
			m_keyPressDelayTimer = 0.0;
	}
	
	// Down key check.
	if ((m_keyPressDownDelayTimer <= 0.0) && (wd::Input::IsKeyDown(wd::Input::Key::Down)))
	{
		MoveDown(cells);
		m_keyPressDownDelayTimer = m_keyPressDownDelay;
	}
	else
	{
		m_keyPressDownDelayTimer = 0.0;

		// Fall by falling timer if the time has come.
		m_fallTimer -= timeDelta;

		if (m_fallTimer <= 0.0)
			MoveDown(cells);
	}
}

void wd::Figure::ReleaseCells(
	wd::Figure::CellMatrix& cells)
{
	// Move cells from the internal member to the cell matrix of the scene.
	for (std::size_t i = 0; i < 4; ++i)
	{
		const glm::u32vec2 cellPos = glm::u32vec2(
			m_position.x + m_possibleSets[m_curSetIndex][i].x,
			m_position.y + m_possibleSets[m_curSetIndex][i].y);

		cells[cellPos.y][cellPos.x] = std::move(m_cells[i]);
	}
}

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------

void wd::Figure::FillCellSets(
	void)
{
	switch (m_type)
	{
		case Type::I:
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(0, 2),
				glm::i32vec2(0, 3),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(-1, 0),
				glm::i32vec2(0, 0),
				glm::i32vec2(1, 0),
				glm::i32vec2(2, 0),
			});
			break;

		case Type::J:
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(-1, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(0, 2),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(-1, 0),
				glm::i32vec2(1, 0),
				glm::i32vec2(-1, 1),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(0, 2),
				glm::i32vec2(1, 2),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 1),
				glm::i32vec2(-1, 1),
				glm::i32vec2(1, 1),
				glm::i32vec2(1, 0),
			});
			break;

		case Type::L:
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(1, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(0, 2),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(-1, 0),
				glm::i32vec2(-1, 1),
				glm::i32vec2(0, 1),
				glm::i32vec2(1, 1),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(0, 2),
				glm::i32vec2(-1, 2),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(-1, 0),
				glm::i32vec2(0, 0),
				glm::i32vec2(1, 0),
				glm::i32vec2(1, 1),
			});
			break;

		case Type::O:
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(1, 0),
				glm::i32vec2(1, 1),
				glm::i32vec2(0, 1),
			});
			break;

		case Type::S:
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(-1, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(1, 1),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(-1, 1),
				glm::i32vec2(-1, 2),
			});
			break;

		case Type::T:
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(-1, 1),
				glm::i32vec2(0, 1),
				glm::i32vec2(1, 1),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(-1, 1),
				glm::i32vec2(0, 2),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(-1, 0),
				glm::i32vec2(1, 0),
				glm::i32vec2(0, 1),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(1, 1),
				glm::i32vec2(0, 2),
			});
			break;

		case Type::Z:
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(1, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(-1, 1),
			});
			m_possibleSets.push_back(
			{
				glm::i32vec2(0, 0),
				glm::i32vec2(0, 1),
				glm::i32vec2(1, 1),
				glm::i32vec2(1, 2),
			});
			break;

		default:
			// Probably new figure type was added.
			// Please, define its rotation states and color.
			assert(false);
			break;
	}
}

void wd::Figure::CreateCells(
	void)
{
	// Create 4 cells with the color, defined by the type of the figure.
	for (std::size_t i = 0; i < 4; ++i)
	{
		m_cells[i] = std::make_unique<wd::Cell>(
			m_colors[static_cast<std::size_t>(m_type)]);
	}
}

void wd::Figure::UpdateCells(
	void)
{
	for (std::size_t i = 0; i < 4; ++i)
	{
		const glm::i32vec2 cellPos = glm::i32vec2(
			m_position.x + m_possibleSets[m_curSetIndex][i].x,
			m_position.y + m_possibleSets[m_curSetIndex][i].y);

		m_cells[i]->SetPosition(cellPos);
	}
}

bool wd::Figure::MoveLeft(
	const CellMatrix& cells)
{
	// We are at the left border - return.
	if (m_position.x == 0)
		return false;

	// Define target scene cell positions.
	const glm::i32vec2 targetPos = glm::i32vec2(m_position.x - 1, m_position.y);
	const wd::Figure::CellSet targetCellPositions = CellPositionsRelToAbs(
		m_possibleSets[m_curSetIndex],
		targetPos);

	// Check for overlapping.
	const bool overlap = IsOverlapped(targetCellPositions, cells);
	if (overlap)
		return false;

	// Update figure position at success and update cells.
	SetPosition(std::move(targetPos));

	return true;
}

bool wd::Figure::MoveRight(
	const CellMatrix& cells)
{
	if (m_position.x == cells[0].size() - 1)
		return false;

	const glm::i32vec2 targetPos = glm::i32vec2(m_position.x + 1, m_position.y);
	const wd::Figure::CellSet targetCellPositions = CellPositionsRelToAbs(
		m_possibleSets[m_curSetIndex],
		targetPos);

	const bool overlap = IsOverlapped(targetCellPositions, cells);
	if (overlap)
		return false;

	SetPosition(std::move(targetPos));

	return true;
}

bool wd::Figure::MoveDown(
	const CellMatrix& cells)
{
	// When we move down, reset the falling timer.
	m_fallTimer = m_fallInterval;

	// Figure as at the bottom - deactivate.
	if (m_position.y == 0)
	{
		Deactivate();
		return false;
	}

	const glm::i32vec2 targetPos = glm::i32vec2(m_position.x, m_position.y - 1);
	const wd::Figure::CellSet targetCellPositions = CellPositionsRelToAbs(
		m_possibleSets[m_curSetIndex],
		targetPos);

	// Figure overlaps something if moves down - deactivate.
	const bool overlap = IsOverlapped(targetCellPositions, cells);
	if (overlap)
	{
		Deactivate();
		return false;
	}

	// Move the figure down.
	SetPosition(std::move(targetPos));

	return true;
}

bool wd::Figure::Shift(
	const wd::Figure::CellMatrix& cells)
{
	// Target cell set (figure orientation).
	const std::size_t targetSetIndex = (m_curSetIndex >= m_possibleSets.size() - 1) ? 0 : m_curSetIndex + 1;
	// Begin with current figure position.
	glm::i32vec2 targetPos = m_position;

	bool overlap;

	// Check current position.
	{
		const wd::Figure::CellSet targetCellPositions = CellPositionsRelToAbs(
			m_possibleSets[targetSetIndex],
			targetPos);

		overlap = IsOverlapped(targetCellPositions, cells);
	}

	// If it overlapped, then...
	if (overlap)
	{
		// Try to move the figure up to 2 cells to the right.
		for (std::size_t offset = 0; offset <= 2; ++offset)
		{
			++targetPos.x;

			const wd::Figure::CellSet targetCellPositions = CellPositionsRelToAbs(
				m_possibleSets[targetSetIndex],
				targetPos);

			overlap = IsOverlapped(targetCellPositions, cells);
			if (!overlap)
				break;
		}
		// If failed again, try to move the figure up to 2 cells to the left.
		if (overlap)
		{
			targetPos = m_position;
			for (std::size_t offset = 0; offset <= 2; ++offset)
			{
				--targetPos.x;

				const wd::Figure::CellSet targetCellPositions = CellPositionsRelToAbs(
					m_possibleSets[targetSetIndex],
					targetPos);

				overlap = IsOverlapped(targetCellPositions, cells);
				if (!overlap)
					break;
			}
		}
	}
	// If the shifted figure overlaps surrounding cells and borders anyway, then return false.
	if (overlap)
		return false;

	// Otherwise set target orientation as the current one,
	// update position if necessary
	// and update cells.
	m_curSetIndex = targetSetIndex;
	m_position = std::move(targetPos);
	UpdateCells();

	// Shift is successful.
	return true;
}

bool wd::Figure::IsOverlapped(
	const wd::Figure::CellSet& cellSet,
	const wd::Figure::CellMatrix& cells)
{
	for (const auto& cellPos : cellSet)
	{
		// Check that cell is not too far left or down.
		if (cellPos.x < 0)
			return true;
		if (cellPos.y < 0)
			return true;

		// Check right bounds of the game field.
		// TODO: Make temp var.
		if (cellPos.y >= static_cast<std::int32_t>(cells.size()))
			return true;
		if (cellPos.x >= static_cast<std::int32_t>(cells[cellPos.y].size()))
			return true;

		// Check the target cell position.
		if (cells[cellPos.y][cellPos.x])
			return true;
	}
	return false;
}

void wd::Figure::Deactivate(
	void)
{
	m_active = false;
}

wd::Figure::CellSet wd::Figure::CellPositionsRelToAbs(
	const wd::Figure::CellSet& relCellSet,
	const glm::i32vec2& figurePos)
{
	wd::Figure::CellSet absCellSet;

	for (std::size_t i = 0; i < 4; ++i)
		absCellSet[i] = figurePos + relCellSet[i];

	return absCellSet;
}
