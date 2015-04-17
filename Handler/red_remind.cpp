#include <stdio.h>

#include "redis_proxy.h"
#include "../Util/mysql_dao.h"
#include "ms_flag.h"
#include "red_remind.h"
#include "glog/logging.h"
#include "../Service/push_client.h"

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
        ret = _redis_proxy->connect(FLAGS_db_redis_host.c_str(), FLAGS_db_redis_port);
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
        vector<Info> ios_info_list;
        vector<Info> android_info_list;
        BatchInfoRequest android_batchinfo_request;
        BatchInfoRequest ios_batchinfo_request;
        //info.type = remind_type;
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
        map<int, int>::iterator it = user_map.begin();
        //for (int i = 0; i < uids_str.size(); i++) {
        for (; it != user_map.end();++it) {
            int32_t uid = it->first;
            vector<int32_t>  device_type;
            vector<string> xg_device_token;

            int ret = _mysql_dao->get_device_info(uid, device_type, xg_device_token);
            if (ret == 0) {
                for (int j = 0; j < xg_device_token.size(); j++) {
                    int32_t dtype = device_type.at(j);
                    Info info;
                    info.device_id = xg_device_token.at(j);
                    info.num = it->second;
                    info.type = remind_type;
                    if (dtype == 1)
                        android_info_list.push_back(info);
                    else if (dtype == 2)
                        ios_info_list.push_back(info);
                }
            }
            int android_list_size = android_info_list.size();
            int ios_list_size = ios_info_list.size();

            if (android_list_size > 0 && android_list_size % FLAGS_batch_push_num == 0) {
                android_batchinfo_request.info_list = android_info_list;
                android_batchinfo_request.device_type = 1;
                _push_client->batch_info(android_batchinfo_request);
                android_info_list.clear();
            }

            if (ios_list_size > 0 && ios_list_size % FLAGS_batch_push_num == 0) {
                ios_batchinfo_request.info_list = ios_info_list;
                ios_batchinfo_request.device_type = 2;
                _push_client->batch_info(ios_batchinfo_request);
                ios_info_list.clear();
            }
            //LOG(INFO) << "key=" << key << " uid=" << uid << " token.size=" << xg_device_token.size() << endl;
        }
        //LOG(INFO) << "[send red] key = " << key << " len = " << uids_str.size() << endl;


        //android_batchinfo_request.device_id_list = android_device_list;
        android_batchinfo_request.info_list = android_info_list;
        android_batchinfo_request.device_type = 1;
        if (android_batchinfo_request.info_list.size() > 0) {
            //LOG(INFO) << "android batch info" << endl;
            _push_client->batch_info(android_batchinfo_request);
        }

        //ios_batchinfo_request.device_id_list = ios_device_list;
        ios_batchinfo_request.info_list = ios_info_list;
        ios_batchinfo_request.device_type = 2;
        if (ios_batchinfo_request.info_list.size() > 0) {
            //LOG(INFO) << "ios batch info" << endl;
            _push_client->batch_info(ios_batchinfo_request);
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
            _red_remind(1);
            _red_remind(2);
            sleep(time_interval);
        }
    }
}
