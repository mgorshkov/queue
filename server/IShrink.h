#pragma once

class IShrink
{
public:
    virtual ~IShrink() = default;

    virtual void Shrink() = 0;
};
