#pragma once
#include "ActionResponse.h"
#include <string>

namespace dnd_game
{
class TextActionResponse : public ActionResponse
{
public:
    using Text = std::string;
    TextActionResponse(const Text& a_reply);
    TextActionResponse(const TextActionResponse& a_other) = delete;
    TextActionResponse& operator=(const TextActionResponse& a_other) = delete;
    ~TextActionResponse() = default;

    virtual ReplyType GetResponse() override;

private:
    Data m_reply;
    size_t m_dataSize;
};
}  // namespace dnd_game