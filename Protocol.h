#pragma once

class Protocol
{
public:
    enum class Message
    {
        QueueList,
    };

    static void Serialize(tcp::socket aSocket, Message::QueueList);
    static void Deserialize(tcp::socket aSocket, Message::QueueList);

    static void Serialize(tcp::socket aSocket, Message::StartQueueSession, aQueueName, aOffset);

    static Item Deserialize(tcp::socket aSocket, Message::Dequeue);
};
