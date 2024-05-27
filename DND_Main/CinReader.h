#pragma once

#include <iostream>
#include "DND_lib/Reader.h"

namespace game_server
{
class CinReader : public dnd_game::Reader
{
public:
	explicit CinReader(std::istream& a_inStream);
	CinReader(const CinReader& a_other) = delete;
	CinReader& operator=(const CinReader& a_other) = delete;
	~CinReader() = default;

	virtual std::string ReadLine() const override;

private:
	std::istream& m_inStream;
};

}
