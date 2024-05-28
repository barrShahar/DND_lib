#include "TextActionResponse.h"

namespace dnd_game
{
TextActionResponse::TextActionResponse(const Text& a_reply)
	: m_reply { std::make_unique<char[]>(a_reply.size() + 1) }  // extra character for the terminate char
	, m_dataSize { a_reply.size() }
{
	// Ctor
	/*std::copy(a_reply.begin(), a_reply.end(), m_reply.get());*/
	size_t i = 0;
	for (; i < a_reply.size(); ++i)
	{
		m_reply[i] = a_reply[i];
	}
	m_reply[i] = '\0';
}

ReplyType TextActionResponse::GetResponse()
{
	return std::move(std::make_pair(std::move(m_reply), m_dataSize));
}

}  // dnd_game