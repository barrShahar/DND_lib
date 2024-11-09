#pragma once
#include <boost/asio.hpp>
#include <string>
#include "DND_lib/Reader.h"

class NetReader: public dnd_game::Reader
{
public:
	NetReader(boost::asio::ip::tcp::socket& socket);
	virtual std::string ReadLine() const override;


private:
	boost::asio::ip::tcp::socket& m_socket;
};

/*
#pragma once
#include "DND_lib/Reader.h"
#include "SimpleNetMT.h"

class NetReader : public dnd_game::Reader
{
public:
	explicit NetReader(simplenet::SimpleNetMT::Connection& a_connection);
	NetReader(const NetReader& a_other) = delete;
	const NetReader& operator=(const NetReader& a_other) = delete;
	~NetReader() = default;

	virtual std::string ReadLine() const override;

private:
	simplenet::SimpleNetMT::Connection& m_connection;


};


*/