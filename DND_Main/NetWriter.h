#pragma once
#include "DND_lib/Writer.h"
#include "SimpleNetMT.h"

class NetWriter : public dnd_game::Writer
{
public:

	explicit NetWriter(simplenet::SimpleNetMT::Connection& a_connection);
	NetWriter(const NetWriter& a_other) = delete;
	const NetWriter& operator=(const NetWriter& a_other) = delete;
	~NetWriter() = default;

	virtual void Write(const std::string& a_str) override;
	virtual void Write(const char a_char) override;
	virtual void Endl() override;

private:
	simplenet::SimpleNetMT::Connection& m_connection;
};

