#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "Connection/SocketConnection.h"
#include <string>
#include <boost/shared_ptr.hpp>


namespace Net
{
class IClientProtocol;
namespace Client
{
    class TcpClient
    {
    public:
        TcpClient( 
            IClientProtocol* theProtocol,
            Reactor::Reactor* theReactor, 
            Processor::BoostProcessor* theProcessor);
        ~TcpClient();

        /**
         * connect in a async way.
         * return:
         *         -1 : there is an error.
         *         0  : connected or blocked
         */
        int connect(const std::string& thePeerAddr, const int thePeerPort);
		int close();
        Net::Buffer::BufferStatus sendn(char* const theBuffer, const size_t theLen);


        /**
         * onXxx
         * Description: TcpClient will be notified if it is xxx.
         */
        void onConnected(int theFd, Connection::SocketConnectionPtr theConnection);
        void onError();

    private:
        IClientProtocol* protocolM;
        Reactor::Reactor* reactorM;
        Processor::BoostProcessor* processorM;
        
        std::string peerAddrM;
        int peerPortM;

        int confReconTimesM;
        int confReconIntervelM;
        int leftReconTimesM;
        int nextReconIntervelM;

        Net::Connection::SocketConnectionPtr connectionM;
    };

    inline Net::Buffer::BufferStatus 
    TcpClient::sendn(char* const theBuffer, const size_t theLen)
    {
        if (connectionM.get())    
        {
            return connectionM->sendn(theBuffer, theLen);
        }
        else
        {
            return Net::Buffer::BufferNotEnoughE; 
        }

    }
}
}

#endif /* TCPCLIENT_H */

