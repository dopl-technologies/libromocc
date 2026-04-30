#ifndef ROMOCC_CLIENT_H
#define ROMOCC_CLIENT_H

#include <mutex>
#include <string>
#include <thread>

#include "romocc/core/Object.h"
#include "zmq.h"
#include "romoccExport.hpp"

namespace romocc
{

class ROMOCC_EXPORT Client : public Object
{
    ROMOCC_OBJECT(Client)

    public:
        typedef unsigned char byte;

        explicit Client();
        ~Client();

        bool requestConnect(std::string ip_address, int port);
        bool requestDisconnect();

        bool isConnected();
        bool sendPackage(std::string package);

        struct ConnectionInfo
        {
            std::string host;
            int port;
        };

    private:
        int getMessageSize(unsigned char* buffer);

        ConnectionInfo mConnectionInfo;
        std::unique_ptr<std::thread> mThread;
        bool mStopThread = false;
        bool mConnected = false;

        // Guards access to mStreamer between sendPackage callers (typically the
        // main control thread) and the start() reader thread, which drains
        // mStreamer to keep its TCP receive window open. ZMQ sockets are not
        // thread-safe; external synchronization is required when the same
        // socket is touched from multiple threads.
        std::mutex mStreamerMutex;

        void* mStreamer;
        void start();
        bool requestReply();
};

}

#endif //ROMOCC_CLIENT_H
