#ifndef __SHARED_HASH_MAP_H_
#define __SHARED_HASH_MAP_H_

#include <time.h>
#include <map>
#include <string>

#include "glog/logging.h"
#include "macro.h"
#include "thread_safe.h"

using std::map;
using std::string;
using std::endl;

namespace tis {

typedef map<int32_t, string> HASHMAP;

class SharedHashMap {

public:

    static const int NOT_INIT = -1;
    static const int PROCESS_ERROR = -2;
    static const int PROCESS_OK = 0;

    SharedHashMap() : _inited(false) {
        for (int i = 0; i < SHARD_NUM; i++) {
            _shard[i] = NULL;
        }
    }

    ~SharedHashMap() {
        if (_inited) {
            for (int i = 0; i < SHARD_NUM; i++) {
                delete _shard[i];
                delete _rw[i];
            }
        }
    }

    int init() {
        for (int i = 0; i < SHARD_NUM; i++ ) {
            _shard[i] = new HASHMAP();
            _rw[i] = new ReadWriteLock();
        }
        _inited = true;
        return 0;
    }

    int find(int32_t uid, string &value) {
        if (!_inited) {
            LOG(ERROR) << "not inited!" << endl;
            return NOT_INIT;
        }
        int sid = which_shard(uid);
        ScopedReadLock lock(_rw[sid]);
        HASHMAP::iterator iter = _shard[sid]->find(uid);
        if (_shard[sid]->end() == iter) {
            return PROCESS_ERROR;
        } else {
            value.assign(iter->second);
            return PROCESS_OK;
        }
    }

    int insert(int32_t uid, string &value) {
        if (!_inited) {
            LOG(ERROR) << "not inited!" << endl;
            return NOT_INIT;
        }
        int sid = which_shard(uid);
        ScopedWriteLock lock(_rw[sid]);
        string temp(value);
        _shard[sid]->insert(std::pair<int32_t, string>(uid, temp));
        return PROCESS_OK;
    }

    int update(int32_t uid, string &value) {
        if (!_inited) {
            LOG(ERROR) << "not inited!" << endl;
            return NOT_INIT;
        }
        int sid = which_shard(uid);
        ScopedWriteLock lock(_rw[sid]);
        HASHMAP::iterator iter = _shard[sid]->find(uid);
        if (_shard[sid]->end() == iter) {
            string temp(value);
            _shard[sid]->insert(std::pair<int32_t, string>(uid, temp));
        } else {
            (iter->second).assign(value);
        }
        return PROCESS_OK;
    }

    int remove(int32_t uid) {
        if (!_inited) {
            LOG(ERROR) << "not inited!" << endl;
            return NOT_INIT;
        }
        int sid = which_shard(uid);
        ScopedWriteLock lock(_rw[sid]);
        _shard[sid]->erase(uid);
        return PROCESS_OK;
    }

private:
    static const int SHARD_NUM = 16;

    int which_shard(const int32_t uid) const {
        return uid % SHARD_NUM;
    }

    bool             _inited;
    HASHMAP*         _shard[SHARD_NUM];
    ReadWriteLock*   _rw[SHARD_NUM];

    DISALLOW_COPY_AND_ASSIGN(SharedHashMap);
};

}   // namespace lj
#endif
