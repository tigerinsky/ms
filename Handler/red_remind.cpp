#include <stdio.h>

#include "redis_proxy.h"
#include "Util/mysql_dao.h"
#include "ms_flag.h"
#include "red_remind.h"
#include "glog/logging.h"
#include "Service/push_client.h"

using std::string;
using std::endl;
using std::vector;
using std::map;

namespace tis {
    RedRemind::RedRemind() {

        _push_client = new PushClient(FLAGS_ps_host.c_str(), FLAGS_ps_port);

        _night_interval = FLAGS_night_interval;
        _day_interval = FLAGS_day_interval;
        _msg_interval = FLAGS_msg_interval;
    }

    int RedRemind::init() {
        _mysql_dao = new MysqlDAO();
        int ret =  _mysql_dao->init(FLAGS_mysql_host.c_str(),
                   FLAGS_mysql_port,
                   FLAGS_mysql_user.c_str(),
                   FLAGS_mysql_pwd.c_str(),
                   FLAGS_mysql_db.c_str());
        if (ret != 0) {
            LOG(ERROR) << "red remind connect mysql error!" << endl;
            return 1;
        }

        _redis_proxy = new RedisProxy();
        ret = _redis_proxy->connect(FLAGS_cache_redis_host.c_str(), FLAGS_cache_redis_port);
        if (ret != 0) {
            LOG(ERROR) << "red remind connect redis error!" << endl;
            return 2;
        }

        return 0;
    }

    RedRemind::~RedRemind() {
        if (NULL != _push_client) {
            delete _push_client;
            _push_client = NULL;
        }

        if (NULL != _mysql_dao) {
            delete _mysql_dao;
            _mysql_dao = NULL;
        }
        if (NULL != _redis_proxy) {
            delete _redis_proxy;
            _redis_proxy = NULL;
        }
    }

    /**获取uid=>消息数键值对
     *
     */
    int RedRemind::_get_remind_map(string key, map<int, int>& user_map) {
        vector<string> uids_str;
        vector<string> score_vec;
        if (RedisProxy::REDIS_ZRANGE_OK != _redis_proxy->zrange(key.c_str(), 0, -1, &uids_str, true, &score_vec)) {
            LOG(ERROR) << "[_red_remind] get key:"<<key<<" error"<< endl;
            return 1;
        }

        for (int i = 0; i < uids_str.size(); i++) {
            int uid = atoi(uids_str.at(i).c_str());
            int score = atoi(score_vec.at(i).c_str());
            user_map[uid] += score;
        }

        return 0;
    }

    void RedRemind::_red_remind(int remind_type) {
        string key;
        map<int, int> user_map;
        if (remind_type == 1) {
            _get_remind_map("ms:index", user_map);
        } else if (remind_type == 2) {
            _get_remind_map("ms:tweet", user_map);
        } else if (remind_type == 6) {
            _get_remind_map("ms:msg", user_map);
            _get_remind_map("ms:pmsg", user_map);
            _get_remind_map("ms:friend", user_map);
        }

        //vector<string>::iterator pos;
        //vector<string> android_device_list;
        //vector<string> ios_device_list;
        //for(pos = uids_str.begin(); pos != uids_str.end(); pos++) {
        MessageType message_type;
        map<int, int>::iterator it = user_map.begin();
        //for (int i = 0; i < uids_str.size(); i++) {
        for (; it != user_map.end();++it) {
            int32_t uid = it->first;
            vector<int32_t>  device_type;
            vector<string> xg_device_token;
            vector<int32_t> src;

            int ret = _mysql_dao->get_device_info(uid, device_type, xg_device_token);
            if (ret == 0) {
                Notify red_remind;
                red_remind.mtype = message_type.FRIENDRED;
                red_remind.num = 1;
                for (int j = 0; j < xg_device_token.size(); j++) {
                    SingleNotifyRequest single_notify_request;
                    single_notify_request.notify = red_remind;
                    single_notify_request.device_id = xg_device_token.at(j);
                    single_notify_request.device_type = device_type.at(j);
                    LOG(INFO) << "friend red device[" << xg_device_token.at(j) << "] device_type[" << device_type.at(j) << "]" << endl;
                    //推送小红点
                    _push_client->single_notify(single_notify_request);
                }
            }
        }

    }
    int RedRemind::_get_hour() {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        
        return timeinfo->tm_hour;
    }

    void RedRemind::red_remind_msg() {
        LOG(INFO) << "red remind msg\n";
        while (1) {
            _red_remind(6);
            LOG(INFO) << "msg red remind time interval:"<<_msg_interval<<endl;
            sleep(_msg_interval);
        }
    }

    void RedRemind::red_remind_bottom() {
        LOG(INFO)<<"red remind bottom\n";
        int time_interval;
        int hour;

        while (1) {
            hour = _get_hour();
            if (hour >=0 && hour <7) {
                time_interval = _night_interval;
            }
            else {
                time_interval = _day_interval;
            }
            LOG(INFO) << "bottom red remind time interval:"<<time_interval<<endl;
            //_red_remind(1);
            _red_remind(2);
            sleep(time_interval);
        }
    }
}
