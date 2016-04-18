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
	m_pCurFigure = std::move(m_pNextFigure);
	m_pNextFigure = std::make_unique<wd::Figure>(1.0 / m_speed);

	m_speed += m_speedIncrement;

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
	if (m_waitingForStart)
	{
		if (wd::Input::IsKeyDown(wd::Input::Key::StartGame))
		{
			RestartGame();
		}
	}
	else
	{
		if (!m_checkFullRows)
		{
			m_pCurFigure->Update(timeDelta, m_cellMatrix);

			if (!m_pCurFigure->IsActive())
			{
				m_pCurFigure->ReleaseCells(m_cellMatrix);
				CheckLoseCondition();
				m_checkFullRows = true;
			}
		}

		bool animationIsPlayed = false;

		// Loop through the cells.
		for (std::size_t row = 0; row < m_fieldSize.y; ++row)
		{
			for (std::size_t col = 0; col < m_fieldSize.x; ++col)
			{
				auto& pCell = m_cellMatrix[row][col];

				if (!pCell)
					continue;

				// Update actor and all of its components.
				// If it is already dead, it won't be updated.
				pCell->Update(timeDelta);

				switch (pCell->GetStatus())
				{
					case wd::Cell::Status::Dead:
						pCell.reset();
						//ShiftCells(row, col);
						continue;
						break;

					case wd::Cell::Status::Dying:
						animationIsPlayed = true;
						break;
				}
			}
		}
		if (!animationIsPlayed)
		{
			if (m_checkFullRows)
			{
				FillEmptyRows();
				CheckFullRows();

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

	for (const auto& cell : m_borders)
	{
		// Draw cell.
		m_pVideoDevice->RenderCell(
			cell.GetPosition(),
			cell.GetColor());
	}

	m_pVideoDevice->RenderText(
		"Счёт:",
		glm::vec2(0.75f, 0.87f),
		glm::vec3(0.8, 0.8f, 0.8f));

	m_pVideoDevice->RenderText(
		std::to_string(m_score),
		glm::vec2(0.75f, 0.83f),
		glm::vec3(0.95, 0.95f, 0.95f));

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
			auto& pCell = m_cellMatrix[row][col];

			if (!pCell)
				continue;

			// Draw cell.
			m_pVideoDevice->RenderCell(
				pCell->GetPosition(),
				pCell->GetColor());
		}
	}

	if (m_waitingForStart)
	{
		RenderWaitingState();
	}
	else
	{
		RenderGameState(timeDelta);
	}

	m_pVideoDevice->PresentFrame();
}

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------

void wd::Scene::CreateBorders(
	void)
{
	for (std::int32_t row = -1; row < static_cast<std::int32_t>(m_fieldSize.y); ++row)
	{
		m_borders.emplace_back(wd::Cell(
			glm::vec3(0.07f, 0.07f, 0.07f),
			glm::i32vec2(-1, row)));

		m_borders.emplace_back(wd::Cell(
			glm::vec3(0.07f, 0.07f, 0.07f),
			glm::i32vec2(m_fieldSize.x, row)));
	}

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
			glm::vec2(0.75f, 0.55f),
			glm::vec3(0.8, 0.2f, 0.2f));
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
			if (!pCell)
			{
				rowIsFull = false;
				break;
			}
		}
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
		// TEMP!!! Debug only.
		if (!pCell)
			continue;

		pCell->Kill();
	}
	m_score += m_fieldSize.x;
	m_emptyRows.emplace_back(row);
}

void wd::Scene::FillEmptyRows(
	void)
{
	while (!m_emptyRows.empty())
	{
		const std::size_t emptyRow = m_emptyRows.back();
		m_emptyRows.pop_back();

		for (std::size_t col = 0; col < m_fieldSize.x; ++col)
		{
			for (std::size_t row = emptyRow + 1; row < m_fieldSize.y; ++row)
			{
				if (!m_cellMatrix[row][col])
					continue;

				m_cellMatrix[row - 1][col] = std::move(m_cellMatrix[row][col]);
				m_cellMatrix[row - 1][col]->SetPosition(glm::i32vec2(col, row - 1));
			}
		}
	}
	m_emptyRows.clear();
}
