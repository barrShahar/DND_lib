#include "NetWriter.h"

NetWriter::NetWriter(simplenet::SimpleNetMT::Connection & a_connection)
	: m_connection { a_connection }
{
}

void NetWriter::Write(const std::string & a_str)
{
	m_connection.Write(a_str);
}

void NetWriter::Write(const char a_char)
{
	m_connection.Write(std::string { a_char });
}

void NetWriter::Endl()
{
	m_connection.Write("\n");
}
