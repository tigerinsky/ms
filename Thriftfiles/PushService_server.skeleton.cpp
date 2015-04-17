// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "PushService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::tis;

class PushServiceHandler : virtual public PushServiceIf {
 public:
  PushServiceHandler() {
    // Your initialization goes here
  }

  void ping() {
    // Your implementation goes here
    printf("ping\n");
  }

  void single_notify(const SingleNotifyRequest& request) {
    // Your implementation goes here
    printf("single_notify\n");
  }

  void batch_notify(const BatchNotifyRequest& request) {
    // Your implementation goes here
    printf("batch_notify\n");
  }

  void admin_notify(const AdminNotifyRequest& request) {
    // Your implementation goes here
    printf("admin_notify\n");
  }

  void batch_info(const BatchInfoRequest& request) {
    // Your implementation goes here
    printf("batch_info\n");
  }

  void broadcast(std::string& _return, const BroadcastRequest& request) {
    // Your implementation goes here
    printf("broadcast\n");
  }

  void zip() {
    // Your implementation goes here
    printf("zip\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<PushServiceHandler> handler(new PushServiceHandler());
  shared_ptr<TProcessor> processor(new PushServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
