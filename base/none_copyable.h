#pragma once

class NoneCopyable
{
public:
    NoneCopyable(const NoneCopyable &) = delete;
    NoneCopyable &operator=(const NoneCopyable &) = delete;
protected:
    NoneCopyable() = default;
    ~NoneCopyable() = default;
};
