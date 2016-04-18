#include "inc/stdafx.h"

#include "Scene.h"

#include "Application/Application.h"
#include "Input/Input.h"
#include "VideoDevice/VideoDevice.h"

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------

// How many cells should be on top of the game field for new figures to spawn.
const std::size_t wd::Scene::m_enterZoneSize = 4;

//-----------------------------------------------------------------------------
// Constructors and destructors
//-----------------------------------------------------------------------------

wd::Scene::Scene(
	const glm::u32vec2& fieldSize,
	wd::VideoDevice* const pVideoDevice)
	:
	// Params.
	m_fieldSize(fieldSize),
	m_pVideoDevice(pVideoDevice),
	// Defaults.
	m_startingSpeed(1.0),
	m_speed(m_startingSpeed),
	m_speedIncrement(0.01),
	m_score(0),
	m_checkFullRows(false),
	m_waitingForStart(true),
	m_gameOver(false)
{
	CreateBorders();
}

wd::Scene::~Scene(
	void)
{
}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

/*
wd::Cell* wd::Scene::CreateActor(const std::string& name)
{
	wd::Cell* const pActor = wd::ActorFactory::Get()->CreateActor(name);
	AddActor(pActor);

	return pActor;
}
*/

void wd::Scene::CreateFigure(
	void)
{
	// Move next figure to the current one.
	m_pCurFigure = std::move(m_pNextFigure);
	// Create the next figure.
	m_pNextFigure = std::make_unique<wd::Figure>(1.0 / m_speed);

	m_speed += m_speedIncrement;

	// If no current figure at the pointer, repeat the method.
	if (!m_pCurFigure)
		CreateFigure();
	else
	{
		// Place new figure at the center-top of the field.
		m_pCurFigure->SetPosition(glm::i32vec2(m_fieldSize.x / 2, m_fieldSize.y));
		// NOTE:: Hardcoded next figure position.
		m_pNextFigure->SetPosition(glm::i32vec2(m_fieldSize.x + 4, 14));
	}
}

void wd::Scene::SetStartingSpeed(
	const double& speed)
{
	m_startingSpeed = speed;
}

void wd::Scene::SetSpeedIncrement(
	const double& increment)
{
	m_speedIncrement = increment;
}

// One iteration of the game logic.
void wd::Scene::StepLogic(
	const double& timeDelta)
{
	// Waiting for the game to start.
	if (m_waitingForStart)
	{
		if (wd::Input::IsKeyDown(wd::Input::Key::StartGame))
		{
			RestartGame();
		}
	}
	// The game is running.
	else
	{
		// We are not checking for the full rows at the moment.
		if (!m_checkFullRows)
		{
			m_pCurFigure->Update(timeDelta, m_cellMatrix);
			// If figure hit something at the bottom...
			if (!m_pCurFigure->IsActive())
			{
				// Move its cells to the cel matrix of the scene.
				m_pCurFigure->ReleaseCells(m_cellMatrix);
				// Check entering zone - maybe some cells are up there and we just lost.
				CheckLoseCondition();
				// Set the flag to check full rows (if we haven't lose).
				if (!m_gameOver)
					m_checkFullRows = true;
			}
		}

		// This is to implement a delay for the moment when we animate the dying cells.
		// Assume that no animation is played now.
		bool animationIsPlayed = false;

		// Loop through the cells.
		for (std::size_t row = 0; row < m_fieldSize.y; ++row)
		{
			for (std::size_t col = 0; col < m_fieldSize.x; ++col)
			{
				// Shorter name.
				auto& pCell = m_cellMatrix[row][col];
				// Empty cell - skip it.
				if (!pCell)
					continue;

				// Update the cell.
				pCell->Update(timeDelta);
				// Check cell's status.
				switch (pCell->GetStatus())
				{
					case wd::Cell::Status::Dead:
						// If it is dead, reset the pointer.
						pCell.reset();
						break;

					case wd::Cell::Status::Dying:
						// If it is dying, start the animation
						animationIsPlayed = true;
						break;
				}
			}
		}
		// During the animation we won't do any checks.
		// But when the animation is not playing...
		if (!animationIsPlayed)
		{
			// If we need to check for full rows...
			if (m_checkFullRows)
			{
				// Shift the cells at the freed rows.
				FillEmptyRows();
				// Check for the full rows.
				CheckFullRows();

				// No more free rows found - continue the game.
				// Create the figure.
				if (!m_checkFullRows)
					CreateFigure();
			}
		}
	}
}

void wd::Scene::Draw(
	const double& timeDelta)
{
	// TODO: Make configurable.
	m_pVideoDevice->ClearBuffer(glm::vec4(0.2f, 0.2f, 0.22f, 1.0f));

	// Draw borders.
	for (const auto& cell : m_borders)
	{
		// Draw cell.
		m_pVideoDevice->RenderCell(
			cell.GetPosition(),
			cell.GetColor());
	}

	// Score.
	m_pVideoDevice->RenderText(
		"Счёт:",
		glm::vec2(0.75f, 0.87f),
		glm::vec3(0.8, 0.8f, 0.8f));

	m_pVideoDevice->RenderText(
		std::to_string(m_score),
		glm::vec2(0.75f, 0.83f),
		glm::vec3(0.95, 0.95f, 0.95f));

	// Control hint.
	m_pVideoDevice->RenderText(
		"Стрелки перемещают",
		glm::vec2(0.75f, 0.42f),
		glm::vec3(0.8, 0.8f, 0.8f));

	m_pVideoDevice->RenderText(
		"фигуру, пробел",
		glm::vec2(0.75f, 0.39f),
		glm::vec3(0.8, 0.8f, 0.8f));

	m_pVideoDevice->RenderText(
		"поворачивает её.",
		glm::vec2(0.75f, 0.36f),
		glm::vec3(0.8, 0.8f, 0.8f));

	// Loop through the lying cells.
	for (std::size_t row = 0; row < m_cellMatrix.size(); ++row)
	{
		for (std::size_t col = 0; col < m_fieldSize.x; ++col)
		{
			// Shorter name.
			auto& pCell = m_cellMatrix[row][col];
			// Empty cell - skip it.
			if (!pCell)
				continue;

			// Draw cell.
			m_pVideoDevice->RenderCell(
				pCell->GetPosition(),
				pCell->GetColor());
		}
	}

	// Now we render the stuff which depends on the game state.
	if (m_waitingForStart)
	{
		RenderWaitingState();
	}
	else
	{
		RenderGameState(timeDelta);
	}
	// Swap the buffers.
	m_pVideoDevice->PresentFrame();
}

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------

void wd::Scene::CreateBorders(
	void)
{
	// Create vertical borders.
	for (std::int32_t row = -1; row < static_cast<std::int32_t>(m_fieldSize.y); ++row)
	{
		// Left.
		m_borders.emplace_back(wd::Cell(
			glm::vec3(0.07f, 0.07f, 0.07f),
			glm::i32vec2(-1, row)));
		// Right.
		m_borders.emplace_back(wd::Cell(
			glm::vec3(0.07f, 0.07f, 0.07f),
			glm::i32vec2(m_fieldSize.x, row)));
	}
	// Create the bottom border.
	for (std::int32_t col = 0; col < static_cast<std::int32_t>(m_fieldSize.x); ++col)
	{
		m_borders.emplace_back(wd::Cell(
			glm::vec3(0.07f, 0.07f, 0.07f),
			glm::i32vec2(col, -1)));
	}
}

void wd::Scene::InitCellMatrix(
	void)
{
	m_cellMatrix.clear();

	// Initialize the cell matrix with empty pointers.
	m_cellMatrix.resize(m_fieldSize.y + m_enterZoneSize);
	for (auto& cellRow : m_cellMatrix)
		cellRow.resize(m_fieldSize.x);
}

void wd::Scene::RenderWaitingState(
	void)
{
	if (m_gameOver)
	{
		m_pVideoDevice->RenderText(
			"ИГРА ОКОНЧЕНА! :-(",
			glm::vec2(0.75f, 0.65f),
			glm::vec3(0.8, 0.2f, 0.2f));

		if (m_score >= 1000)
		{
			m_pVideoDevice->RenderText(
				"Но Вы набрали 1000+ очков!",
				glm::vec2(0.75f, 0.60f),
				glm::vec3(0.2, 0.8f, 0.2f));
			m_pVideoDevice->RenderText(
				"Поздравляю!",
				glm::vec2(0.75f, 0.57f),
				glm::vec3(0.2, 0.8f, 0.2f));
		}
	}

	m_pVideoDevice->RenderText(
		"Для начала игры",
		glm::vec2(0.75f, 0.50f),
		glm::vec3(0.8, 0.8f, 0.8f));
	m_pVideoDevice->RenderText(
		"нажмите Enter.",
		glm::vec2(0.75f, 0.47f),
		glm::vec3(0.8, 0.8f, 0.8f));
}

void wd::Scene::RenderGameState(
	const double& timeDelta)
{
	// Delay the falling of the figure, is animation is played.
	if (!m_checkFullRows)
	{
		{
			const auto& figureCells = m_pCurFigure->GetCells();
			for (const auto& pCell : figureCells)
			{
				// Draw cell.
				m_pVideoDevice->RenderCell(
					pCell->GetPosition(),
					pCell->GetColor());
			}
		}
		{
			const auto& figureCells = m_pNextFigure->GetCells();
			for (const auto& pCell : figureCells)
			{
				// Draw cell.
				m_pVideoDevice->RenderCell(
					pCell->GetPosition(),
					pCell->GetColor());
			}
		}
	}

	m_pVideoDevice->RenderText(
		"Следующая:",
		glm::vec2(0.75f, 0.77f),
		glm::vec3(0.8, 0.8f, 0.8f));
}

void wd::Scene::GameOver(
	void)
{
	m_gameOver = true;
	m_waitingForStart = true;
}

void wd::Scene::RestartGame(
	void)
{
	m_speed = m_startingSpeed;
	m_score = 0;
	m_checkFullRows = false;
	m_gameOver = false;
	m_waitingForStart = false;

	InitCellMatrix();
	m_pCurFigure.reset();
	m_pNextFigure.reset();
	CreateFigure();
}

void wd::Scene::CheckLoseCondition(
	void)
{
	// Check the entering zone (4 rows above the game field).
	// If there is a cell, we lost. :-(
	for (std::size_t row = m_fieldSize.y; row < m_cellMatrix.size(); ++row)
	{
		for (const auto& pCell : m_cellMatrix[row])
		{
			if (pCell)
			{
				GameOver();
				return;
			}
		}
	}
}

void wd::Scene::CheckFullRows(
	void)
{
	m_checkFullRows = false;

	for (std::size_t row = 0; row < m_fieldSize.y; ++row)
	{
		bool rowIsFull = true;

		for (const auto& pCell : m_cellMatrix[row])
		{
			// Found an empty place in the row - the row is not full, skip it.
			if (!pCell)
			{
				rowIsFull = false;
				break;
			}
		}
		// Row is full - destroy it.
		if (rowIsFull)
		{
			m_checkFullRows = true;
			KillRow(row);
		}
	}
}

void wd::Scene::KillRow(
	const std::size_t& row)
{
	for (auto& pCell : m_cellMatrix[row])
	{
		pCell->Kill();
	}
	// Add 1 point for each cell in the row.
	m_score += m_fieldSize.x;
	// Add the row index to the list of empty rows
	// which should be filled with cells from the top rows.
	m_emptyRows.emplace_back(row);
}

void wd::Scene::FillEmptyRows(
	void)
{
	while (!m_emptyRows.empty())
	{
		// The list of empty row indices is filled from the bottom to the top (0...N).
		// But to properly shift the top rows down we should move from the top free rows to the bottom ones.
		const std::size_t emptyRow = m_emptyRows.back();
		m_emptyRows.pop_back();

		for (std::size_t col = 0; col < m_fieldSize.x; ++col)
		{
			for (std::size_t row = emptyRow + 1; row < m_fieldSize.y; ++row)
			{
				if (!m_cellMatrix[row][col])
					continue;

				// Swap cells.
				m_cellMatrix[row - 1][col] = std::move(m_cellMatrix[row][col]);
				// Set new position for the cell.
				m_cellMatrix[row - 1][col]->SetPosition(glm::i32vec2(col, row - 1));
			}
		}
	}
	// Clear the list of empty row indices.
	m_emptyRows.clear();
}
