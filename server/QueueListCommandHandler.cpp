#include <iostream>

#include "insertcommandhandler.h"

InsertCommandHandler::InsertCommandHandler(ITableManager* aTableManager)
    : CommandHandler(aTableManager)
{		
}

std::string InsertCommandHandler::GetCommand() const
{
    return "INSERT";
}

CompleteCommand InsertCommandHandler::Parse(const std::string& aLine)
{
#ifdef DEBUG_PRINT
    std::cout << "InsertCommandHandler::Parse, aLine=" << aLine << std::endl;
#endif

    auto posId = aLine.find(' ');
    if (posId == std::string::npos)
        return CompleteCommand{Command::Error};

    std::string tableName = aLine.substr(0, posId);

#ifdef DEBUG_PRINT
    std::cout << "InsertCommandHandler::Parse, tableName=" << tableName << std::endl;
#endif

    auto posName = aLine.find(' ', posId + 1);
    if (posName == std::string::npos)
        return CompleteCommand{Command::Error};

    TableRow row;
    row.mId = std::atoi(aLine.substr(posId + 1, posName - posId + 1).c_str());
    row.mName = aLine.substr(posName + 1, aLine.length() - posName + 1);

#ifdef DEBUG_PRINT
    std::cout << "InsertCommandHandler::Parse, tableRow=" << row << std::endl;
#endif

    return CompleteCommand{Command::Insert, tableName, row};
}

CompleteOperationStatus InsertCommandHandler::Handle(const CompleteCommand& aCommand)
{
    return mTableManager->Insert(aCommand.mTableName, aCommand.mRow);    
}

