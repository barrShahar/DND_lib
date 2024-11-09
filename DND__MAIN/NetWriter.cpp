// NetWriter.cpp
#include "NetWriter.h"
#include <iostream>

NetWriter::NetWriter(boost::asio::ip::tcp::socket& socket)
    : m_socket(socket)
{
}

void NetWriter::Write(const std::string& message)
{
    try
    {
        boost::asio::write(m_socket, boost::asio::buffer(message + "\n"));
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "Error writing message: " << e.what() << std::endl;
    }
}

void NetWriter::Write(const char a_char)
{
    NetWriter::Write(std::string{ a_char });
}
