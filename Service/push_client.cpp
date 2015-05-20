#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "glog/logging.h"

//#include "PushService.h"
#include "push_client.h"

namespace tis {
    void PushClient::ping(std::string& _return) {
        try {
            transport->open();
            client.ping(_return);
        } catch (TException& tx) {
            LOG(WARNING) << "ping exception, msg["<<tx.what()<<"]";
        }
        if (NULL != transport) {
            transport->close();
        }
    }

    int32_t PushClient::single_notify(const SingleNotifyRequest& request) {
        int32_t ret = -1;
        try {
            transport->open();
            ret = client.single_notify(request);
        } catch (TException& tx) {
            LOG(WARNING) << "single_notify exception, msg["<<tx.what()<<"]";
        }
        if (NULL != transport) {
            transport->close();
        }

        return ret;
    }


    int32_t PushClient::broadcast(const BroadcastRequest& request) {
        int32_t ret = -1;
        try {
            transport->open();
            ret = client.broadcast(request);
        } catch (TException& tx) { 
            LOG(WARNING) << "broadcast exception, msg["<<tx.what()<<"]";
        }
        if (NULL != transport) {
            transport->close();
        }

        return ret;
    }

    int32_t PushClient::batch_notify(const BatchNotifyRequest& request) {
        int32_t ret = -1;
        try {
            transport->open();
            ret = client.batch_notify(request);
        } catch (TException& tx) {
            LOG(WARNING) << "batch_notify exception, msg["<<tx.what()<<"]";
        }
        if (NULL != transport) {
            transport->close();
        }

        return ret;
    }

    int32_t PushClient::optag(const TagRequest& request) {
        int32_t ret = -1;
        try {
            transport->open();
            ret = client.optag(request);
        } catch (TException& tx) {
            LOG(WARNING) << "optag exception, msg[" << tx.what() << "]";
        }

        if (NULL != transport) {
            transport->close();
        }

        return ret;
    }

    void PushClient::condition_push(const ConditionPushRequest& request) {
        try {
            transport->open();
            client.condition_push(request);
        } catch (TException& tx) {
            LOG(WARNING) << "condition_push exception, msg[" << tx.what() << "]";
        }

        if (NULL != transport) {
            transport->close();
        }
    }
};
