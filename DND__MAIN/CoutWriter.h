#pragma once
#include "DND_lib/Writer.h"
namespace game_server
{
class CoutWriter : public dnd_game::Writer
{
public:
	explicit CoutWriter(std::ostream& a_os);
	CoutWriter(const CoutWriter& a_other) = delete;
	CoutWriter& operator=(const CoutWriter& a_other) = delete;
	~CoutWriter() = default;

	virtual void Write(const std::string& a_str) override;
	virtual void Write(const char a_char) override;
	virtual void Endl() override;

private:
	std::ostream& m_os;
};
} // game_server


