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
        boost::shared_ptr<TTransport> socket;
        boost::shared_ptr<TTransport> transport;
        boost::shared_ptr<TProtocol> protocol;
        PushServiceClient client;

    public:
        PushClient(const char* host, const int port) :socket(new TSocket(host, port)), transport(new TFramedTransport(socket)), protocol(new TBinaryProtocol(transport)), client(protocol) {
        };
        void ping(string& _return);
        int32_t single_notify(const SingleNotifyRequest& request);
        int32_t batch_notify(const BatchNotifyRequest& request);
        int32_t broadcast(const BroadcastRequest& request);
        int32_t optag(const TagRequest& request);
        void condition_push(const ConditionPushRequest& request);

        ~PushClient() {
            if (NULL != transport) {
                transport->close();
            }
        };
};
};
#endif
