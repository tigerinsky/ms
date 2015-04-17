#ifndef push_client_H
#define push_client_H

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include "../Thriftfiles/push_server_types.h"
#include "../Thriftfiles/PushService.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

namespace tis {

class PushClient {
    private:
        //boost::shared_ptr<apache::thrift::transport::TTransport> socket;
        //boost::shared_ptr<apache::thrift::transport::TTransport> transport;
        //boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol;
        boost::shared_ptr<TTransport> socket;
        boost::shared_ptr<TTransport> transport;
        boost::shared_ptr<TProtocol> protocol;
        PushServiceClient client;

    public:
        PushClient(const char* host, const int port) :socket(new TSocket(host, port)), transport(new TFramedTransport(socket)), protocol(new TBinaryProtocol(transport)), client(protocol) {//TODO:host port从配置读
            //transport->open();
        };
        void ping();
        void single_notify(const SingleNotifyRequest& request);
        void batch_notify(const BatchNotifyRequest& request);
        void admin_notify(const AdminNotifyRequest& request);
        void batch_info(const BatchInfoRequest& request);
        void broadcast(std::string& _return, const BroadcastRequest& request);

        ~PushClient() {
            if (NULL != transport) {
                transport->close();
            }
        };
};
};
#endif
