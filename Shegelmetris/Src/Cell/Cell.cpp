#include "inc/stdafx.h"

#include "Cell.h"

#include "Scene/Scene.h"

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------

const glm::vec3 wd::Cell::m_deadColor = glm::vec3(1.0f, 1.0f, 1.0f);
const double wd::Cell::m_dyingTime = 0.3;

//-----------------------------------------------------------------------------
// Constructors and destructors
//-----------------------------------------------------------------------------

wd::Cell::Cell(
	const glm::vec3& color,
	const glm::vec2& position)
	:
	// Const members.
	m_initialColor(color),
	m_color(color),
	m_position(position),
	// Defaults.
	m_status(wd::Cell::Status::Alive),
	m_animationTimer(0.0)
{
}

wd::Cell::Cell(
	const glm::vec3& color,
	const glm::i32vec2& position)
	:
	wd::Cell::Cell(
		color,
		static_cast<glm::vec2>(position))
{
}

wd::Cell::Cell(
	const glm::vec3& color)
	:
	wd::Cell::Cell(
		color,
		glm::vec2(0.0f, 0.0f))
{
}

wd::Cell::~Cell(
	void)
{
}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

const wd::Cell::Status& wd::Cell::GetStatus(
	void) const
{
	return m_status;
}

void wd::Cell::SetStatus(
	const wd::Cell::Status& status)
{
	m_status = status;
}

const glm::vec2& wd::Cell::GetPosition(
	void) const
{
	return m_position;
}

void wd::Cell::SetPosition(
	const glm::vec2& position)
{
	m_position = position;
}

void wd::Cell::SetPosition(
	const glm::i32vec2& position)
{
	SetPosition(glm::vec2(static_cast<float>(position.x), static_cast<float>(position.y)));
}

const glm::vec3& wd::Cell::GetColor(
	void) const
{
	return m_color;
}

void wd::Cell::Update(
	const double& timeDelta)
{
	if (m_status == wd::Cell::Status::Dying)
	{
		m_animationTimer += timeDelta;

		if (m_animationTimer >= m_dyingTime)
		{
			SetStatus(wd::Cell::Status::Dead);
			return;
		}
		m_color = glm::lerp(m_initialColor, m_deadColor, static_cast<float>(m_animationTimer / m_dyingTime));
	}
}

void wd::Cell::Kill(
	void)
{
	assert(m_status == wd::Cell::Status::Alive);

	SetStatus(wd::Cell::Status::Dying);
}

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------
