#pragma once
#include "ActionResponse.h"
namespace dnd_game
{
class RnStringActionResponse : public ActionResponse
{
public:
    explicit RnStringActionResponse(const std::string& a_reply);
    RnStringActionResponse(const RnStringActionResponse& a_other) = delete;
    RnStringActionResponse& operator=(const RnStringActionResponse& a_other) = delete;
    ~RnStringActionResponse() = default;

    virtual ReplyType GetResponse() override;

private:
    Data m_reply;
    size_t m_dataSize;
};
}	// namespace dnd_game



