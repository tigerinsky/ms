#include <string>
#include <sys/time.h>
#include <stdlib.h>
#include "glog/logging.h"
#include "ms_flag.h"
#include "redis_proxy.h"
#include "expired_queue.h"

using std::string;
using std::endl;
namespace tis {
    bool ExpiredQueue::is_exist(const char* key,
            const char* value, uint32_t size) {
        bool ret = false;
        string score;
        if (RedisProxy::REDIS_ZSCORE_OK ==
                _redis_proxy->zscore(key, value, size, score)) {
            int64_t cur_time = get_timesec();
            int64_t item_time = atoi(score.c_str());
            if (cur_time - item_time < FLAGS_expire_time_in_second) {
                ret = true;
            }
            //LOG(INFO) << "item_time=" << item_time \
            //    << " score=" << score.c_str() \
            //    << " cur_time=" << cur_time << endl;
        }
        return ret;
    }

    int ExpiredQueue::add(const char* key, const char* value, uint32_t size, uint64_t max_len) {
        //先检查key对应的队列是否已经满了
        uint64_t cur_len = get_count(key);
        if (cur_len >= max_len) {
            rem_expired(key, FLAGS_expired_queue_rem_step);//队列满时删除20
        }

        int64_t score = get_timesec();
        if (RedisProxy::REDIS_ZADD_OK ==
                _redis_proxy->zadd(key, value, size, score)) {
            return 0;
        } else {
            return -1;
        }
    }

    int64_t ExpiredQueue::get_timesec() {
        timeval _timeval;
        gettimeofday(&_timeval, NULL);
        return _timeval.tv_sec;
    }

    uint64_t ExpiredQueue::get_count(const char* key) {
        uint64_t len = 0;
        if (RedisProxy::REDIS_ZCARD_OK == 
                _redis_proxy->zcard(key, &len)) {
            return len;
        } else {
            return 0;
        }
    }

    int ExpiredQueue::rem_expired(const char* key, int32_t num) {
        _redis_proxy->zremrangebyrank(key, 0, num);
        return 0;
    }
}
