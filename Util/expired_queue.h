#ifndef __EXPIRED_QUEUE_H_
#define __EXPIRED_QUEUE_H_

namespace tis {

class RedisProxy;

class ExpiredQueue {
public:
    ExpiredQueue(RedisProxy* redis): 
        _redis_proxy(redis) {
    }
    ~ExpiredQueue() {
    }

    bool is_exist(const char* key, const char* value, uint32_t size);
    int add(const char* key, const char* value, uint32_t size, uint64_t max_len);

private:
    uint64_t get_count(const char* key);
    int rem_expired(const char* key, int32_t num);
    int64_t get_timesec();

private:
    RedisProxy* _redis_proxy;
};

}
#endif
