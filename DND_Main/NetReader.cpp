// NetReader.cpp
#include "NetReader.h"
#include <iostream>


NetReader::NetReader(boost::asio::ip::tcp::socket& a_socket)
    : m_socket(a_socket)
{
    // Constructor: Initialize the NetReader with a reference to the socket
}

std::string NetReader::ReadLine() const
{
    // Create a buffer to hold the incoming data
    boost::asio::streambuf buffer;
    boost::system::error_code ec;

    // Read from the socket until a newline character is found
    boost::asio::read_until(m_socket, buffer, '\n', ec);

    // Check for errors in reading
    if (ec && ec != boost::asio::error::eof)
    {
        // Print the error message if there was an issue reading from the socket
        std::cerr << "Error reading from socket: " << ec.message() << std::endl;
        return "";  // Return an empty string in case of an error
    }

    // Convert the buffer into an input stream
    std::istream stream(&buffer);
    std::string line;
    // Read a line from the stream
    std::getline(stream, line);

    // Check if the line ends with '\r' and remove it
    if (!line.empty() && line.back() == '\r') {
        line.erase(line.size() - 1);
    }

    return line;  // Return the line that was read
}
