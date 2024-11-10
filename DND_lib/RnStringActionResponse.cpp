#include "RnStringActionResponse.h"
#include <string>

namespace dnd_game
{

    // Utility function to replace all occurrences of '\n' with "\r\n"
    std::string ReplaceNewlineWithCRLF(const std::string& input)
    {
        std::string result;
        for (char c : input)
        {
            if (c == '\n')
            {
                result += "\r\n";
            }
            else
            {
                result += c;
            }
        }
        return result;
    }

    RnStringActionResponse::RnStringActionResponse(const std::string& a_reply)
        : m_dataSize{ ReplaceNewlineWithCRLF(a_reply).size() } // Adjust data size based on processed string
    {
        std::string processedReply = ReplaceNewlineWithCRLF(a_reply);
        m_reply = std::make_unique<char[]>(m_dataSize + 1);

        std::copy(processedReply.begin(), processedReply.end(), m_reply.get());
        m_reply[m_dataSize] = '\0'; // Null-terminate the string
    }

    ReplyType RnStringActionResponse::GetResponse()
    {
        return std::move(std::make_pair(std::move(m_reply), m_dataSize));
    }

}  // namespace dnd_game
