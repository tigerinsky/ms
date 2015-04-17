#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "glog/logging.h"

//#include "PushService.h"
#include "push_client.h"

namespace tis {
    void PushClient::ping() {
        try {
            transport->open();
            client.ping();
        } catch (TException& tx) {
            LOG(WARNING) << "ping exception, msg["<<tx.what()<<"]";
        }
        if (NULL != transport) {
            transport->close();
        }
    }

    void PushClient::single_notify(const SingleNotifyRequest& request) {
        try {
            transport->open();
            client.single_notify(request);
        } catch (TException& tx) {
            LOG(WARNING) << "single_notify exception, msg["<<tx.what()<<"]";
        }
        if (NULL != transport) {
            transport->close();
        }
    }


    void PushClient::batch_info(const BatchInfoRequest& request) {
        try {
            transport->open();
            client.batch_info(request);
        } catch (TException& tx) {
            LOG(WARNING) << "batch_info exception, msg["<<tx.what()<<"]";
        }

        if (NULL != transport) {
            transport->close();
        }
    }

    void PushClient::admin_notify(const AdminNotifyRequest& request) {
        try {
            transport->open();
            client.admin_notify(request);
        } catch (TException& tx) {
            LOG(WARNING) << "admin_notify exception, msg["<<tx.what()<<"]";
        }
        if (NULL != transport) {
            transport->close();
        }
    }

    void PushClient::broadcast(std::string& _return, const BroadcastRequest& request) {
        try {
            transport->open();
            client.broadcast(_return, request);
        } catch (TException& tx) { 
            LOG(WARNING) << "broadcast exception, msg["<<tx.what()<<"]";
        }
        if (NULL != transport) {
            transport->close();
        }
    }

    void PushClient::batch_notify(const BatchNotifyRequest& request) {
        try {
            transport->open();
            client.batch_notify(request);
        } catch (TException& tx) {
            LOG(WARNING) << "batch_notify exception, msg["<<tx.what()<<"]";
        }
        if (NULL != transport) {
            transport->close();
        }
    }
};
