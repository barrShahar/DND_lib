#include "CinReader.h"

namespace game_server
{
CinReader::CinReader(std::istream & a_inStream) : m_inStream { a_inStream }
{
	// Constructor
}

std::string CinReader::ReadLine() const
{
	std::string returnValue;
	std::getline(m_inStream, returnValue);
	return returnValue;
}

}

