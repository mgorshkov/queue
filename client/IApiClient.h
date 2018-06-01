#pragma once

class IApiClient
{
public:
    virtual ~IApiClient() = default;

    virtual void Connect(const ServerData& aServerData) = 0;
};
