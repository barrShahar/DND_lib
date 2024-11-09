#include "NetReader.h"
#include <iostream>
#include <boost/asio.hpp>

NetReader::NetReader(boost::asio::ip::tcp::socket& socket)
    : m_socket(socket)
{
}

std::string NetReader::ReadLine() const
{
    boost::asio::streambuf buffer;
    try
    {
        boost::asio::read_until(m_socket, buffer, "\n");
        std::istream input_stream(&buffer);
        std::string line;
        std::getline(input_stream, line);
        return line;
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "Error reading line: " << e.what() << std::endl;
        return "";
    }
}


/*
#include "NetReader.h"

NetReader::NetReader(simplenet::SimpleNetMT::Connection & a_connection)
	: m_connection { a_connection }
{
}

std::string NetReader::ReadLine() const
{
	return m_connection.ReadLine();
}

*/