#ifndef __RED_REMIND_H__
#define __RED_REMIND_H__

#include <map>
#include <string>

using std::map;
using std::string;

namespace tis {

class RedisProxy;
class PushClient;
class MysqlDAO;

class RedRemind {
    public:
        RedRemind();
        ~RedRemind();
        void red_remind_bottom();
        void red_remind_msg();
        int init();
    private:
        void _red_remind(int remind_type);
        int _get_remind_map(string key, map<int, int> &use_map);
        int _get_hour();

    private:
        RedisProxy *_redis_proxy;
        MysqlDAO *_mysql_dao;
        PushClient * _push_client;
        int _night_interval;
        int _day_interval;
        int _msg_interval;
};
}

#endif
