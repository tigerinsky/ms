#include <sys/time.h>
#include <string>
#include <unistd.h>
#include <pthread.h>

#include "redis_proxy.h"
#include "glog/logging.h"
#include "../Service/push_client.h"
#include "../Util/mysql_dao.h"
#include "ms_flag.h"
#include "message_server_handler.h"
#include "../Util/constant.h"
#include "../Util/expired_queue.h"
//#include "config.pb.h"
#include "../Util/cJSON.h"

using std::string;
using std::endl;
using std::vector;
using namespace ::apache::thrift;
//using namespace lj;

namespace tis {
    typedef struct ThreadSpace {
        RedisProxy *_redis_proxy;
        RedisProxy *_cache_redis_proxy;
        MysqlDAO *_mysql_dao;
        PushClient *_push_client;
        ExpiredQueue *_expired_queue;
    } ThreadSpace;
    ThreadSpace* g_thread_space = NULL;
    pthread_mutex_t g_space_mutex;
    int g_thread_index = 0;
    __thread int space_id = -1;

    static int __thread_init(ThreadSpace* space) {
        space->_mysql_dao = new MysqlDAO();
        int ret = space->_mysql_dao->init(FLAGS_mysql_host.c_str(),
                FLAGS_mysql_port,
                FLAGS_mysql_user.c_str(),
                FLAGS_mysql_pwd.c_str(),
                FLAGS_mysql_db.c_str());
        if (ret != 0) { //数据库初始化失败,启动退出
            return 1;
        }
        space->_redis_proxy = new RedisProxy();//离线
        ret = space->_redis_proxy->connect(FLAGS_db_redis_host.c_str(),
                FLAGS_db_redis_port);

        space->_cache_redis_proxy = new RedisProxy();//用户
        int ret2 = space->_cache_redis_proxy->connect(FLAGS_cache_redis_host.c_str(),
                FLAGS_cache_redis_port);

        if (0 != ret || 0 != ret2) {
            return 2;
        }
        space->_expired_queue = new ExpiredQueue(space->_redis_proxy);
        space->_push_client = new PushClient(FLAGS_ps_host.c_str(), FLAGS_ps_port);

        return 0;

    }

    static ThreadSpace* __get_thread_space() {
        //LOG(INFO) << "space id:" << space_id << endl;
        if (-1 != space_id) {
            return g_thread_space + space_id;
        }
        pthread_mutex_lock(&g_space_mutex);
        space_id = g_thread_index++;
        pthread_mutex_unlock(&g_space_mutex);

        return g_thread_space + space_id;
    }

    int MessageServerHandler::init_thread_data(int thread_num) {
        if (pthread_mutex_init(&g_space_mutex, NULL)) {
                return 1;
        }

        g_thread_space = (ThreadSpace*)malloc(sizeof(ThreadSpace) * thread_num);
        if (NULL == g_thread_space) {
            return 2;
        }

        for (int i = 0; i < thread_num; ++i) {
            if (__thread_init(g_thread_space + i)) {
                return 3;
            }
        }

        return 0;
    }

    long MessageServerHandler::get_timestamp() {
        timeval _timeval;
        gettimeofday(&_timeval, NULL);
        return _timeval.tv_sec;
    }

    string MessageServerHandler::generate_friend_notify_content(vector<int32_t> uids) {
        string content = "好友";
        for (int i = 0; i < uids.size(); i++) {
            int32_t uid = uids.at(i);
            string sname;
            get_username(uid, sname);
            if (i == 0) {
                content.append(sname);
            }
            else if (i <= 2){
                content.append(", " + sname);
            }
            else {
                content.append("等");
                break;
            }
        }
        content.append("加入了蓝鲸");

        return content;
    }

    /*void MessageServerHandler::new_friend_notify(const NewFriendRequest& request) {
        //记录开始时间
        timeval time_start;
        gettimeofday(&time_start, NULL);

        //获取线程变量
        ThreadSpace* tsp = __get_thread_space();
        MysqlDAO* _mysql_dao = tsp->_mysql_dao;
        PushClient* _push_client = tsp->_push_client;
        RedisProxy* _redis_proxy = tsp->_redis_proxy;

        int32_t to_uid = request.to_uid;
        size_t rec_uids_len = request.rec_uids.size();

        //设置小红点
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "%d", to_uid);
        if (RedisProxy::REDIS_ZINCR_OK != _redis_proxy->zincr("ms:friend", buffer, strlen(buffer), rec_uids_len)) {
            LOG(ERROR) << "zincr ms:friend error " << to_uid << endl;
        }//把一个队列拆成三个

        vector<int32_t> device_type;
        vector<string> xg_device_token;
        if (0 == _mysql_dao->get_device_info(to_uid, device_type, xg_device_token)) {
            //LOG(INFO) << "get_device_info ok!" << endl;
            Notify notify;
            Info info;
            NotifyType notify_type;
            InfoType info_type;

            notify.title = "蓝鲸消息"; 
            notify.type = notify_type.FRIEND;
            string config_type("friend_notify");
            //notify.content = "有新的联系人加入";//TODO 临时文案
            notify.content = generate_friend_notify_content(request.rec_uids);
            for (size_t j = 0; j < xg_device_token.size(); j++) {
                 SingleNotifyRequest single_notify_request;
                 single_notify_request.notify = notify;
                 single_notify_request.device_id = xg_device_token.at(j);
                 single_notify_request.device_type = device_type.at(j);
                 vector<Info> info_list;
                 info.device_id =  xg_device_token.at(j);
                 info.type = info_type.NEW_FRIEND;
                 info_list.push_back(info);
                 BatchInfoRequest batch_info_request;
                 batch_info_request.info_list = info_list;
                 batch_info_request.device_type = device_type.at(j);
                 _push_client->batch_info(batch_info_request);
                 if (can_push(to_uid, config_type)) {
                        _push_client->single_notify(single_notify_request);
                 }
            }
        }

        timeval time_end;
        gettimeofday(&time_end, NULL);
        LOG(INFO) << "[new_friend_notify] to_uid=" << to_uid \
            << " new friend num=" << rec_uids_len \
            << " cost=" << TIMEDIFF(time_start, time_end) << endl;

    }*/

    //关注和私信通知蓝鲸小秘书
    /*int MessageServerHandler::admin_notify(int16_t type,
            int16_t flow, int32_t from_uid,
            int32_t to_uid, int32_t ctime,
            int32_t mid) {
        //记录开始时间
        timeval time_start;
        gettimeofday(&time_start, NULL);

        ThreadSpace* tsp = __get_thread_space();
        PushClient* _push_client = tsp->_push_client;

        AdminNotifyRequest request;
        request.type = type;
        request.flow = flow;
        request.from_uid = from_uid;
        request.to_uid = to_uid;
        request.ctime = ctime;
        request.mid = mid;
        _push_client->admin_notify(request);

        timeval time_end;
        gettimeofday(&time_end, NULL);
        LOG(INFO) << "[admin_notify]" \
            << " type=" << type \
            << " from_uid=" << from_uid \
            << " to_uid=" << to_uid \
            << " ctime=" << ctime \
            << " mid=" << mid \
            << " cost=" << TIMEDIFF(time_start, time_end) \
            << endl;

        return 0;
    }*/

    bool MessageServerHandler::check_action_frequent(int32_t from_uid,
            int type, int32_t to_uid, int32_t content_id) {
        if (ActionType::FOLLOW != type && ActionType::PRAISE != type) {
            return false;
        }

        char buf[256];
        bool ret = false;
        snprintf(buf, sizeof(buf), "%d:%d:%d:%d", from_uid, type, to_uid, content_id);
        ThreadSpace* tsp = __get_thread_space();
        ExpiredQueue* _expired_queue = tsp->_expired_queue;
        string key("");
        if (ActionType::FOLLOW == type) {
            key.assign("ms:follow:queue");
        } else if (ActionType::PRAISE == type) {
            key.assign("ms:praise:queue");
        }
        ret = _expired_queue->is_exist(key.c_str(), buf, strlen(buf));
        _expired_queue->add(key.c_str(), buf, strlen(buf), FLAGS_expired_queue_max_len);

        return ret;
    }

    //发送系统消息
    void MessageServerHandler::send_system_msg(const SystemMessage& sMsg) {
        // Your implementation goes here
        timeval time_start;
        gettimeofday(&time_start, NULL);

        ThreadSpace* tsp = __get_thread_space();
        MysqlDAO* _mysql_dao = tsp->_mysql_dao;
        PushClient* _push_client = tsp->_push_client;
        RedisProxy* _redis_proxy = tsp->_redis_proxy;
 
        long time_now = get_timestamp();
        std::string from_name;
        get_username(sMsg.from_uid, from_name);
        MessageType message_type;
        LandingType landing_type;

        timeval t1, t2, t3;
        for (size_t i = 0; i < sMsg.to_uid.size(); i++) {
            std::string to_name;
            get_username(sMsg.to_uid.at(i), to_name);
            if (sMsg.from_uid == sMsg.to_uid.at(i)) {
                continue;
            }

            //检查点赞或者关注是否频繁操作
            if (check_action_frequent(sMsg.from_uid, sMsg.action_type,
                        sMsg.to_uid.at(i), sMsg.content_id)) {
                LOG(INFO) << "action too frequent!" << endl;
                continue;
            }

            //系统消息入数据库(私信除外)
            if (ActionType::MAIL != sMsg.action_type
                    && ActionType::NEW_FRIEND > sMsg.action_type) {
                int ret = _mysql_dao->insert_system_message(sMsg.from_uid, from_name.c_str(),
                        sMsg.action_type, sMsg.to_uid.at(i), to_name.c_str(), sMsg.content_id,
                        time_now, time_now);
                if (0 != ret) {
                    LOG(ERROR) << "insert system message error!" << endl;
                }
            }


            /*//蓝鲸小秘书私信或被关注
            int to_dummy = get_dummy(sMsg.to_uid.at(i));
            if (2 == to_dummy) {//1:后台小马甲; 2:小秘书
                int flow = 1;
                if (ActionType::FOLLOW == sMsg.action_type) {
                    admin_notify(2, flow, sMsg.from_uid, sMsg.to_uid.at(i), time_now, 0);
                } else if (ActionType::MAIL == sMsg.action_type) {
                    string content("");
                    int32_t ctime = 0;
                    _mysql_dao->get_message(sMsg.content_id, content, ctime);
                    admin_notify(1, flow, sMsg.from_uid, sMsg.to_uid.at(i), ctime, sMsg.content_id);
                }
            }*/

            //读数据库ci_user_push得到每个用户的deviceType和xg_device_token. 
            vector<int32_t> device_type;
            vector<string> xg_device_token;

            gettimeofday(&t1, NULL);
            if (0 == _mysql_dao->get_device_info(sMsg.to_uid.at(i), device_type, xg_device_token)) {
                //LOG(INFO) << "get_device_info ok!" << endl;
                gettimeofday(&t2, NULL);
                Notify notify;
                notify.mtype = message_type.NOTIFY;
                notify.title = "美院帮消息"; //TODO 临时文案
                notify.ltype = landing_type.SYSTEM_MSG;
                string config_type = "";

                Notify red_remind;
                red_remind.mtype = message_type.NOTIFYRED;
                red_remind.num = 1;
                switch (sMsg.action_type) {
                    case 0:
                        notify.content = from_name + " @了你";
                        config_type.assign("at_notify");
                        break;
                    case 1:
                        notify.content = from_name + " 给你发了一条私信";
                        notify.ltype = landing_type.PRIVATE_MSG;
                        notify.uid = sMsg.from_uid;
                        red_remind.ltype = message_type.EMAILRED;
                        red_remind.uid = sMsg.from_uid;
                        config_type.assign("pmsg_notify");
                        break;
                    case 2:
                        notify.content = from_name + " 评论了你的帖子";
                        config_type.assign("comment_notify");
                        break;
                    case 3:
                        notify.content = from_name + " 回复了你的评论";
                        config_type.assign("comment_notify");
                        break;
                    case 5:
                        notify.content = from_name + " 关注了你";
                        config_type.assign("follow_notify");
                        break;
                    case 6:
                        notify.content = from_name + "赞了你";
                        config_type.assign("zan_notify");
                        break;
                    case 7:
                        notify.content = "你的帖子被推荐到信息流了";
                        break;
                    /*case 8:
                        notify.content = "有新的联系人加入";
                        notify.type = notify_type.FRIEND;
                        config_type.assign("friend_notify");
                        break;*/
                    case 9:
                        notify.content = "认证通过";
                        notify.ltype = landing_type.INDEX;
                        break;
                    default:
                        break;
                }
                for (size_t j = 0; j < xg_device_token.size(); j++) {
                    SingleNotifyRequest single_notify_request;
                    single_notify_request.notify = red_remind;
                    single_notify_request.device_id = xg_device_token.at(j);
                    single_notify_request.device_type = device_type.at(j);

                    //推送小红点
                    _push_client->single_notify(single_notify_request);

                    //推送通知
                    single_notify_request.notify = notify;
                    if(single_notify_request.device_type == 2 || 
                       (single_notify_request.device_type == 1 && can_push(sMsg.to_uid.at(i), config_type))) {
                        _push_client->single_notify(single_notify_request);
                    }
                }
            }

            //消息入redis队列
            char buffer[128];
            snprintf(buffer, sizeof(buffer), "%d", sMsg.to_uid.at(i));
            if (sMsg.action_type == 1) {//私信队列加1
                string key = "ms:pmsg";
                char from_uid[128];
                snprintf(from_uid, sizeof(from_uid), "%d", sMsg.from_uid);
                key += buffer;
                if (RedisProxy::REDIS_ZINCR_OK != _redis_proxy->zincr(key.c_str(), from_uid, strlen(from_uid), 1)) {
                    LOG(ERROR) << "zincr " << key << " error " << sMsg.from_uid << endl;
                }
            }
            else {//其它系统消息加1
                if (RedisProxy::REDIS_ZINCR_OK != _redis_proxy->zincr("ms:msg", buffer, strlen(buffer), 1)) {
                    LOG(ERROR) << "zincr ms:msg error" << sMsg.to_uid.at(i) << endl;
                }
            }
            gettimeofday(&t3, NULL);
            LOG(INFO) << "get_device_info_cost=" << TIMEDIFF(t1, t2) << " send_push_cost=" << TIMEDIFF(t2, t3) << endl;
        }// end of for (size_t i = 0; i < sMsg.to_uid.size(); i++) 

        timeval time_end;
        gettimeofday(&time_end, NULL);
        LOG(INFO) << "[send_system_msg]" \
            << " f_uid=" << sMsg.from_uid \
            << " fname=" << from_name.c_str() \
            << " a_type=" << sMsg.action_type \
            << " t_uid_len=" << sMsg.to_uid.size()\
            << " content_id=" <<sMsg.content_id \
            << " cost=" << TIMEDIFF(time_start, time_end) \
            << endl;
    }

    int MessageServerHandler::get_username(
            int32_t uid,
            std::string& name) {
        ThreadSpace* tsp = __get_thread_space();
        RedisProxy* _redis_proxy = tsp->_redis_proxy;

        int ret = -1;
        char uid_buf[128];
        snprintf(uid_buf, sizeof(uid_buf), "user_%d", uid);
        if (RedisProxy::REDIS_HGET_OK == 
                _redis_proxy->hget(uid_buf, "sname", name)) {
            ret = 0;
        } else {
            LOG(ERROR) << "get user name error! uid = " << uid << endl;
        }
        return ret;
    }

    int MessageServerHandler::get_dummy(int32_t uid) {
        string dummy("");
        ThreadSpace* tsp = __get_thread_space();
        RedisProxy* _redis_proxy = tsp->_redis_proxy;
        int res = -1;
        char uid_buf[128];
        snprintf(uid_buf, sizeof(uid_buf), "user_%d", uid);
        if (RedisProxy::REDIS_HGET_OK ==
                _redis_proxy->hget(uid_buf, "dummy", dummy)) {
            if (dummy != "") {
                res = atoi(dummy.c_str());
            }
        } else {
            LOG(ERROR) << "get user dummy error! uid = " << uid << endl;
        }
        return res;
    }

    bool MessageServerHandler::can_push(int32_t uid, string config_type) {
        ThreadSpace* tsp = __get_thread_space();
        MysqlDAO* _mysql_dao = tsp->_mysql_dao;

        string config;
        int ret = _mysql_dao->get_push_config(uid, config);
        if (ret != 0) {
            LOG(ERROR) << "get push config error" << endl;
            return false;
        }

        if (config == "") {
            if (config_type == "zan_notify") { //点赞通知默认不推送
                return false;
            }
            return true;
        }

        const char *config_str = config.c_str();
        cJSON *root_json = cJSON_Parse(config_str);
        if (NULL == root_json) {
            LOG(ERROR) << cJSON_GetErrorPtr() << endl;
            return false;
        }
        int result = 1;
        cJSON *is_push = cJSON_GetObjectItem(root_json, config_type.c_str());
        if (is_push != NULL) {
            result = is_push->valueint;
        }
        else {
            if (config_type == "zan_notify") { //点赞通知默认不推送
                result = 0;
            }
        }
        if (root_json) {
            cJSON_Delete(root_json);
        }
        
        return result == 1;
    }

    int32_t MessageServerHandler::get_num(const int32_t uid, const int32_t queue_type) {
        string key;
        long time_start = get_timestamp();
        if (queue_type == 1) {
            key.assign("ms:pmsg");
            char uid_str[128];
            snprintf(uid_str, sizeof(uid_str), "%d", uid);
            key += uid_str;
        }
        else if (queue_type == 2) {
            key.assign("ms:friend");
        }
        else if (queue_type == 3) {
            key.assign("ms:msg");
        }
        else {
            LOG(ERROR) << "queue type error" << endl;
            return 1;
        }
        ThreadSpace* tsp = __get_thread_space();
        RedisProxy* _redis_proxy = tsp->_redis_proxy; 

        uint64_t pmsg_len;
        if (queue_type == 1) {
            if (RedisProxy::REDIS_ZCARD_OK != _redis_proxy->zcard(key.c_str(), &pmsg_len)) {
                LOG(ERROR) << "get pmsg num error" << endl;
                return 0;
            }
            LOG(INFO) <<"[get_red_num]"<<" uid="<<uid<<" key="<<key<<" len="<<pmsg_len<<endl;
             
            return pmsg_len;
        }

        string score;
        char value[100];
        snprintf(value, sizeof(value), "%d", uid);
        LOG(INFO) << "value:" << value << endl;
        int ret =  _redis_proxy->zscore(key.c_str(), value, strlen(value), score);
        if (RedisProxy::REDIS_ZSCORE_OK != ret) {
            if (RedisProxy::REDIS_ZSCORE_ERR == ret) {
                LOG(ERROR) << "get msg num error, ret:" << ret \
                    << " key: " << key.c_str()\
                    << " value: " << value << endl;
            }
            return 0;
        }
        long time_end = get_timestamp();
        LOG(INFO) <<"[get_red_num]"<<" uid="<<value<<" key="<<key<<" score="<<score<<" cost="<<time_end - time_start<<endl;

        return atoi(score.c_str());

    }

    void MessageServerHandler::set_read(const int32_t sysMessageId) {//move to off_hub
        // Your implementation goes here
        timeval time_start;
        gettimeofday(&time_start, NULL);

        ThreadSpace* tsp = __get_thread_space();
        MysqlDAO* _mysql_dao = tsp->_mysql_dao;

        int ret = _mysql_dao->set_system_message_read(
                sysMessageId,
                get_timestamp());

        timeval time_end;
        gettimeofday(&time_end, NULL);
        if (0 != ret) {
            LOG(ERROR) << "set sys_message read error ssysid = "<< sysMessageId <<endl;
        } else {
            LOG(INFO) << "[set_read] messageId=" << sysMessageId \
                << " cost=" << TIMEDIFF(time_start, time_end) << endl;
        }
    }

    void MessageServerHandler::set_delete(const int32_t sysMessageId) {//TODO move to off_hub
        // Your implementation goes here
        timeval time_start;
        gettimeofday(&time_start, NULL);

        ThreadSpace* tsp = __get_thread_space();
        MysqlDAO* _mysql_dao = tsp->_mysql_dao;

        int ret = _mysql_dao->set_system_message_delete(
                sysMessageId,
                get_timestamp());

        timeval time_end;
        gettimeofday(&time_end, NULL);
        if (0 != ret) {
            LOG(ERROR) << "set sys_message delete error ssysid = "<< sysMessageId <<endl;
        } else {
            LOG(INFO) << "[set_delete] messageId=" << sysMessageId \
                << " cost=" << TIMEDIFF(time_start, time_end) << endl;
        }
    }

    //消除小红点
    void MessageServerHandler::clear_red_by_uid(
            const int32_t uid, 
            const int32_t mType,
            const int32_t num,
            const int32_t from_uid) {
        // Your implementation goes here
        timeval time_start;
        gettimeofday(&time_start, NULL);

        ThreadSpace* tsp = __get_thread_space();
        RedisProxy* _redis_proxy = tsp->_redis_proxy;
        
        string key("");
        if (1 == mType) {
            key.assign("ms:index");
        } else if (2 == mType) {
            key.assign("ms:tweet");
        } else if (6 == mType) {
            key.assign("ms:msg");
        } else if (7 == mType) {
            key.assign("ms:friend");
        } else if (8 == mType) {//此时，需要知道具体是消除哪个from_uid的私信
            key.assign("ms:pmsg");
            char uid_str[128];
            snprintf(uid_str, sizeof(uid_str), "%d", uid);
            key += uid_str;
        }
        else {
            LOG(ERROR) << "mType error" << endl;
            return;
        }

        char buffer[128];
        if (8 == mType) {
            snprintf(buffer, sizeof(buffer), "%d", from_uid);
        }
        else {
            snprintf(buffer, sizeof(buffer), "%d", uid);
        }
        _redis_proxy->zrem(key.c_str(), buffer, strlen(buffer));

        timeval time_end;
        gettimeofday(&time_end, NULL);
        LOG(INFO) << "[clear_red] key=" << key.c_str() << " uid=" << uid \
            << " cost=" << TIMEDIFF(time_start, time_end) << endl;
    }

    void MessageServerHandler::update_config(const int32_t key, const string& value) {//TODO config缓存移到redis中
        int32_t uid = key;
        string a = value;
        g_data.g_shared_push_config.update(uid, a);

        string new_value;
        g_data.g_shared_push_config.find(uid, new_value);
        LOG(INFO) << "[update_config] key=" << key << " new_value=" << new_value.c_str() << endl;
    }

    void MessageServerHandler::new_tweet_notify(
            const NewTweetNotifyRequest& request) {
        // Your implementation goes here
        timeval time_start;
        gettimeofday(&time_start, NULL);

        ThreadSpace* tsp = __get_thread_space();
        RedisProxy* _redis_proxy = tsp->_redis_proxy;

        string key("");
        char buffer[128];
        if (TweetType::INDEX == request.type) {
            key.assign("ms:index");
        } else if (TweetType::COMMUNITY == request.type) {
            key.assign("ms:tweet");
        } else {
            LOG(ERROR) << "NewTweetType error" << endl;
            return;
        }
        for (size_t i = 0; i < request.uids.size(); i++) {
            //小红点相关，存到redis中
            snprintf(buffer, sizeof(buffer), "%d", request.uids.at(i));
            if (RedisProxy::REDIS_ZINCR_OK != _redis_proxy->zincr(key.c_str(), buffer, strlen(buffer), 1)) {
                LOG(ERROR) << "zincr error" << request.uids.at(i) << endl;
            }
        }

        timeval time_end;
        gettimeofday(&time_end, NULL);
        LOG(INFO) << "[new_tweet_notify]" \
            << " to_uid_length=" << request.uids.size() \
            << " tweet_type=" << request.type \
            << " tid=" << request.tid \
            << " cost=" << TIMEDIFF(time_start, time_end) \
            << endl;
    }

    void MessageServerHandler::mis_notify(
            const MisRequest& request) {//TODO 通过offhub调用
        // Your implementation goes here
        timeval time_start;
        gettimeofday(&time_start, NULL);

        ThreadSpace* tsp = __get_thread_space();
        MysqlDAO* _mysql_dao = tsp->_mysql_dao;
        PushClient* _push_client = tsp->_push_client;

        int is_broadcast = request.is_broadcast;
        Notify notify;
        notify.mtype = 1;
        notify.ltype = request.type;
        notify.content = request.content;
        notify.title = request.title;

        LandingType ntype;
        if (request.type == ntype.WAP) {
            notify.url = request.url;
        }

        if (request.type == ntype.COMMUNITY_DETAIL) {
            notify.tid = request.tid;
        }

        if (is_broadcast == 0) {
            BroadcastRequest broadcast_request;
            broadcast_request.notify = notify;
            broadcast_request.send_time = request.send_time;
            broadcast_request.push_task_id = request.push_task_id;
            
            _push_client->broadcast(broadcast_request);
        }
        else {//采用tag推送
            ConditionPushRequest condition_request;
            condition_request.notify = notify;
            condition_request.device_type = request.device_type;
            condition_request.city = request.city;
            condition_request.school = request.school;
            condition_request.ukind_verify = request.ukind_verify;
            condition_request.send_time = request.send_time;
            condition_request.push_task_id = request.push_task_id;

            _push_client->condition_push(condition_request);
        }

        timeval time_end;
        gettimeofday(&time_end, NULL);
        /*LOG(INFO) << "[notice_notify]" \
            << " industry_id=" << request.industry_id \
            << " type=" << request.type \
            << " title=" << request.title \
            << " content=" << request.content \
            << " cost=" << TIMEDIFF(time_start, time_end) \
            << endl;*///TODO 后续整理日志
    }

}
