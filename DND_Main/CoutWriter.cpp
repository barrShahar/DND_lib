#include <ostream>
#include "CoutWriter.h"

namespace game_server
{
CoutWriter::CoutWriter(std::ostream& a_os) : m_os { a_os }
{
	// Constructor
}

void CoutWriter::Write(const std::string& a_str) 
{
	m_os << a_str;
}

void CoutWriter::Write(const char a_char) 
{
	m_os << a_char;
}

}	// game_server