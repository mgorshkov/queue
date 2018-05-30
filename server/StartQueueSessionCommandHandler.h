#pragma once

#include "commandhandler.h"
#include "itablemanager.h"

class InsertCommandHandler : public CommandHandler
{
public:
    InsertCommandHandler(ITableManager* aTableManager);

    virtual std::string GetCommand() const override;
    virtual CompleteCommand Parse(const std::string& aLine) override;
    virtual CompleteOperationStatus Handle(const CompleteCommand& aCommand) override;
};
