#include "inc/stdafx.h"

#include "Config.h"

//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------

const wd::Config* wd::Config::m_pConfig = nullptr;

//-----------------------------------------------------------------------------
// Constructors and destructors
//-----------------------------------------------------------------------------

wd::Config::Config(
	const std::string& fileName)
{
	assert(!m_pConfig);
	m_pConfig = this;

	LoadFromXML(fileName);
}

wd::Config::~Config(
	void)
{
	m_pConfig = nullptr;
}

//-----------------------------------------------------------------------------
// Static methods
//-----------------------------------------------------------------------------

const wd::Config* wd::Config::Get(
	void)
{
	return m_pConfig;
}

//-----------------------------------------------------------------------------
// Public methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private methods
//-----------------------------------------------------------------------------

void wd::Config::LoadFromXML(
	const std::string& fileName)
{
	/*
	std::ofstream outStream(fileName, std::fstream::out);

	{
		cereal::XMLOutputArchive outArchive(outStream);
		outArchive(*this);
	}

	outStream.close();
	*/

	// TODO: No file existance check here!
	std::ifstream inStream(fileName, std::fstream::in);

	{
		cereal::XMLInputArchive inArchive(inStream);
		inArchive(*this);
	}

	inStream.close();
}
