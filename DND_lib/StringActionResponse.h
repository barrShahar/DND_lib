#pragma once
#include <string>
#include "ActionResponse.h"

namespace dnd_game
{
class StringActionResponse : public ActionResponse
{
public:
    explicit StringActionResponse(const std::string& a_reply);
    StringActionResponse(const StringActionResponse& a_other) = delete;
    StringActionResponse& operator=(const StringActionResponse& a_other) = delete;
    ~StringActionResponse() = default;

    virtual ReplyType GetResponse() override;
    friend StringActionResponse operator+(const StringActionResponse& lhs, const StringActionResponse& rhs);

private:
    Data m_reply;
    size_t m_dataSize;
};


}	// dnd_game