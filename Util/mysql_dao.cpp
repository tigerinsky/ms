#include <sys/time.h>
#include <unistd.h>

#include "mysql_proxy.h"
#include "constant.h"
#include "glog/logging.h"
#include "../Proto/ms.pb.h"
#include "mysql_dao.h"

using std::endl;

namespace tis {

    int MysqlDAO::init(const char* host, int port,
            const char* user, const char* pwd,
            const char* db) {
        _mysql_proxy = new MysqlProxy();
        int ret = _mysql_proxy->connect(host, port, user, pwd);
        if (MysqlProxy::MYSQL_CONNECT_OK != ret) { //数据库连接失败
            return 1;
        }
        _mysql_proxy->set_charset("utf8");
        _mysql_proxy->select_db(db);

        //初始化send_msg的statment
        _send_msg_stmt = new mysql_stmt_t(); 
        _mysql_proxy->prepare("INSERT INTO ci_system_message (from_uid, from_name, action_type, \
            to_uid, to_name, content_id, ctime, utime) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
            NULL,
            _send_msg_stmt,
            MysqlProxy::PREPARE_INT32,
            MysqlProxy::PREPARE_STRING,
            MysqlProxy::PREPARE_UINT16,
            MysqlProxy::PREPARE_INT32,
            MysqlProxy::PREPARE_STRING,
            MysqlProxy::PREPARE_INT64,
            MysqlProxy::PREPARE_INT64,
            MysqlProxy::PREPARE_INT64);

        //初始化set_read的statement
        _set_read_stmt = new mysql_stmt_t();
        _mysql_proxy->prepare("UPDATE ci_system_message SET is_read = 1, utime = ? WHERE sys_message_id = ?",
                NULL,
                _set_read_stmt,
                MysqlProxy::PREPARE_INT64,
                MysqlProxy::PREPARE_INT32);

        //初始化set_delete的statement
        _set_delete_stmt = new mysql_stmt_t();
        _mysql_proxy->prepare("UPDATE ci_system_message SET is_del = 1, utime = ? WHERE sys_message_id = ?",
                NULL,
                _set_delete_stmt,
                MysqlProxy::PREPARE_INT64,
                MysqlProxy::PREPARE_INT32);

        //初始化_get_device_info的statement
        _get_device_info_stmt = new mysql_stmt_t();
        _user_push = new UserPush();
        //_mysql_proxy->prepare("SELECT xg_device_token, device_type FROM ci_user_push WHERE uid = ?",
        _mysql_proxy->prepare("SELECT * FROM ci_user_push WHERE uid = ?",
                _user_push->GetDescriptor(),
                _get_device_info_stmt,
                MysqlProxy::PREPARE_INT32);

        //初始化get_push_config的statement
        _get_push_config_stmt = new mysql_stmt_t();
        _push_config = new PushConfig();
        _mysql_proxy->prepare("SELECT uid, config FROM ci_config_push WHERE uid = ?",
                _push_config->GetDescriptor(),
                _get_push_config_stmt,
                MysqlProxy::PREPARE_INT32);

        //初始化industry_users的statement
        _get_industry_users_stmt = new mysql_stmt_t();
        _user_industry = new UserIndustry();
        _mysql_proxy->prepare("SELECT * FROM ci_user_industry WHERE industry_id = ? limit ?, ?",
                _user_industry->GetDescriptor(),
                _get_industry_users_stmt,
                MysqlProxy::PREPARE_INT32,
                MysqlProxy::PREPARE_INT32,
                MysqlProxy::PREPARE_INT32);

        //初始化私信message的statement
        _get_message_stmt = new mysql_stmt_t();
        _ci_message = new CiMessage();
        _mysql_proxy->prepare("SELECT mid, content, ctime FROM ci_message WHERE mid = ?",
                _ci_message->GetDescriptor(),
                _get_message_stmt,
                MysqlProxy::PREPARE_INT32);

        return 0;
    }

    void MysqlDAO::destroy() {
        if (NULL != _mysql_proxy) {
            if (NULL != _send_msg_stmt) {
                _mysql_proxy->free_prepare(_send_msg_stmt);
                delete _send_msg_stmt;
            }
            if (NULL != _set_read_stmt) {
                _mysql_proxy->free_prepare(_set_read_stmt);
                delete _set_read_stmt;
            }
            if (NULL != _set_delete_stmt) {
                _mysql_proxy->free_prepare(_set_delete_stmt);
                delete _set_delete_stmt;
            }

            if (NULL != _get_device_info_stmt) {
                _mysql_proxy->free_prepare(_get_device_info_stmt);
                delete _get_device_info_stmt;
            }

            if (NULL != _user_push) {
                _user_push->Clear();
                delete _user_push;
            }

            if (NULL != _get_push_config_stmt) {
                _mysql_proxy->free_prepare(_get_push_config_stmt);
                delete _get_push_config_stmt;
            }

            if (NULL != _push_config) {
                _push_config->Clear();
                delete _push_config;
            }

            if (NULL != _get_industry_users_stmt) {
                _mysql_proxy->free_prepare(_get_industry_users_stmt);
                delete _get_industry_users_stmt;
            }

            if (NULL != _user_industry) {
                _user_industry->Clear();
                delete _user_industry;
            }

            if (NULL != _get_message_stmt) {
                _mysql_proxy->free_prepare(_get_message_stmt);
                delete _get_message_stmt;
            }

            if (NULL != _ci_message) {
                _ci_message->Clear();
                delete _ci_message;
            }

            _mysql_proxy->close();
            delete _mysql_proxy;
            _mysql_proxy = NULL;
        }
    }

    int MysqlDAO::insert_system_message(int32_t from_uid,
            const char* from_name,
            int32_t action_type,
            int32_t to_uid,
            const char* to_name,
            int64_t content_id,
            long create_time,
            long update_time) {
        int ret = _mysql_proxy->execute(
                _send_msg_stmt,
                from_uid,
                from_name,
                action_type,
                to_uid,
                to_name,
                content_id,
                create_time,
                update_time);
        if (MysqlProxy::MYSQL_QUERY_OK != ret) {
            LOG(ERROR) << "insert system message error" <<  _mysql_proxy->get_err_msg() << endl;
        } /*else {
            LOG(INFO) << "send_system_msg" \
                << " f_uid=" << from_uid \
                << " fname=" << from_name \
                << " a_type=" << action_type \
                << " t_uid=" << to_uid \
                << " tname=" << to_name \
                << " content_id=" <<content_id \
                << endl;
        }*/
        _mysql_proxy->free_result(_send_msg_stmt);
        return ret;
    }

    int MysqlDAO::get_device_info(
            int32_t uid,
            vector<int32_t> &device_type, 
            vector<string> &xg_device_token 
            ) {
        int res = 0;
        device_type.clear();
        xg_device_token.clear();

        if (MysqlProxy::MYSQL_QUERY_OK ==
                _mysql_proxy->execute(_get_device_info_stmt, uid)) {
            //LOG(INFO) << "get_device_info execute ok!" << endl;
            bool flag = true;
            while (flag) {
                switch (_mysql_proxy->next(_get_device_info_stmt)) {
                    case MysqlProxy::MYSQL_NEXT_ERR:
                        LOG(ERROR) << "get error " << _mysql_proxy->get_err_msg() << endl;
                        flag = false;
                        res = -1;
                        break;
                    case MysqlProxy::MYSQL_NEXT_END:
                        //LOG(INFO) << "get end" << endl;
                        flag = false;
                        break;
                    case MysqlProxy::MYSQL_NEXT_OK:
                        int ret = _mysql_proxy->get_proto(_get_device_info_stmt, _user_push);
                        if (MysqlProxy::MYSQL_GET_PROTO_OK == ret) {
                            //得到了device_type和xg_device_token
                            device_type.push_back(_user_push->device_type());
                            xg_device_token.push_back(_user_push->xg_device_token().c_str());
                            //LOG(INFO) << "device_type = " << _user_push->device_type()
                            //    << " xg_device_token = " << _user_push->xg_device_token().c_str() 
                            //    << "uid = " << uid << endl;
                        }
                        _user_push->Clear();
                        break;
                }
            }
        } else {
            LOG(ERROR) << "get_device_info error! uid = " << uid << endl;
            res = -1;
        }
        _user_push->Clear();
        _mysql_proxy->free_result(_get_device_info_stmt);
        return res;
    }

    int MysqlDAO::get_push_config(int32_t uid, string &ret_config) {
        int res = 0;
        ret_config.clear();
        int ret = g_data.g_shared_push_config.find(uid, ret_config);
        if (ret == SharedHashMap::PROCESS_OK) {
            //LOG(INFO) << "get config from db: " << ret_config << endl;
            return res;
        }
        ret  = _mysql_proxy->execute(
                _get_push_config_stmt,
                uid);
        if (MysqlProxy::MYSQL_QUERY_OK != ret) {
            _mysql_proxy->free_result(_get_push_config_stmt);
            LOG(ERROR) << _mysql_proxy->get_err_msg() << endl;
            res = -1;
            return res;
        }

        bool not_end = true;
        while (not_end) {
            switch(_mysql_proxy->next(_get_push_config_stmt)) {
                case MysqlProxy::MYSQL_NEXT_ERR:
                    //LOG(INFO) << _mysql_proxy->get_err_msg() << _mysql_proxy->get_prepare_err_msg(_get_push_config_stmt) << endl;
                    not_end = false;
                    res = -1;
                    break;
                case MysqlProxy::MYSQL_NEXT_END:
                    not_end = false;
                    break;
                case MysqlProxy::MYSQL_NEXT_OK:
                    int ret = _mysql_proxy->get_proto(_get_push_config_stmt, _push_config);
                    if (MysqlProxy::MYSQL_GET_PROTO_OK != ret) {
                        LOG(ERROR) << "trans to proto error" << endl;
                        res = -1;
                        not_end = false;
                    }
                    else {
                        ret_config = _push_config->config();
                        //LOG(INFO) << "get config from db: " << ret_config << endl;
                        if (ret_config != "") {
                            g_data.g_shared_push_config.update(uid, ret_config);
                        }
                    }
                    _push_config->Clear();
                    break;
            }
        }
        _mysql_proxy->free_result(_get_push_config_stmt);
        
        return res;
    }

    int MysqlDAO::set_system_message_read(int32_t sys_message_id, long time) {
        //LOG(INFO) << "get sys id = " << sys_message_id << endl;
        int ret = _mysql_proxy->execute(
                _set_read_stmt,
                time,
                sys_message_id);
        if (MysqlProxy::MYSQL_QUERY_OK != ret) {
            LOG(ERROR) << "set system message read error" << _mysql_proxy->get_err_msg() << endl;
        } /*else {
            LOG(INFO) << "set_read : messageId = " << sys_message_id << endl;
        }*/
        _mysql_proxy->free_result(_set_read_stmt);
        return ret;
    }

    int MysqlDAO::set_system_message_delete(int32_t sys_message_id, long time) {
        int ret = _mysql_proxy->execute(
                _set_delete_stmt,
                time,
                sys_message_id);

        if (MysqlProxy::MYSQL_QUERY_OK != ret) {
            LOG(ERROR) << "set system message delete error" << _mysql_proxy->get_err_msg() << endl;
        } /*else {
            LOG(INFO) << "set_delete : messageId = " << sys_message_id << endl;
        }*/
        _mysql_proxy->free_result(_set_delete_stmt);
        return ret;
    }

    /*void MysqlDAO::get_industry_users(const int32_t industry_id, vector<long>& uid_list) {
        int limit_size = 5000;
        int offset = 0;
        vector<long> uids;

        while (1) {
            uids.clear();
            get_industry_users_by_limit(industry_id, uids, offset, limit);
            int usize = uids.size();
            if (usize > 0) {

            }
        }
    }*/

    void MysqlDAO::get_industry_users_by_limit(const int32_t industry_id, vector<long>& uid_list, int offset, int limit) {
        int ret  = _mysql_proxy->execute(
                _get_industry_users_stmt,
                industry_id, offset, limit);
        if (MysqlProxy::MYSQL_QUERY_OK != ret) {
            _mysql_proxy->free_result(_get_industry_users_stmt);
            LOG(INFO) << _mysql_proxy->get_err_msg() << endl;
            return;
        }
        bool not_end = true;
        while (not_end) {
            switch(_mysql_proxy->next(_get_industry_users_stmt)) {
                case MysqlProxy::MYSQL_NEXT_ERR:
                    LOG(ERROR) << _mysql_proxy->get_err_msg() << _mysql_proxy->get_prepare_err_msg(_get_industry_users_stmt) << endl;
                    not_end = false;
                    break;
                case MysqlProxy::MYSQL_NEXT_END:
                    not_end = false;
                    break;
                case MysqlProxy::MYSQL_NEXT_OK:
                    int ret = _mysql_proxy->get_proto(_get_industry_users_stmt, _user_industry);
                    if (MysqlProxy::MYSQL_GET_PROTO_OK != ret) {
                        LOG(ERROR) << "trans to proto error" << endl;
                        not_end = false;
                    } else {
                        uid_list.push_back(_user_industry->uid());
                    }

                    break;
            }
        }

        _user_industry->Clear();
        _mysql_proxy->free_result(_get_industry_users_stmt);
    }

    int MysqlDAO::get_message(const int32_t mid,
            string &content, int32_t &ctime) {
        int res = 0;
        int ret  = _mysql_proxy->execute(
                _get_message_stmt,
                mid);
        if (MysqlProxy::MYSQL_QUERY_OK != ret) {
            _mysql_proxy->free_result(_get_message_stmt);
            LOG(ERROR) << _mysql_proxy->get_err_msg() << endl;
            res = -1;
            return res;
        }
        bool not_end = true;
        while (not_end) {
            switch(_mysql_proxy->next(_get_message_stmt)) {
                case MysqlProxy::MYSQL_NEXT_ERR:
                    LOG(ERROR) << _mysql_proxy->get_err_msg() << _mysql_proxy->get_prepare_err_msg(_get_message_stmt) << endl;
                    not_end = false;
                    res = -1;
                    break;
                case MysqlProxy::MYSQL_NEXT_END:
                    not_end = false;
                    break;
                case MysqlProxy::MYSQL_NEXT_OK:
                    int ret = _mysql_proxy->get_proto(_get_message_stmt, _ci_message);
                    if (MysqlProxy::MYSQL_GET_PROTO_OK != ret) {
                        LOG(ERROR) << "trans to proto error" << endl;
                        not_end = false;
                        res = -1;
                    } else {
                        content.assign(_ci_message->content().c_str());
                        ctime = _ci_message->ctime();
                        _ci_message->Clear();
                    }

                    break;
            }
        }

        _ci_message->Clear();
        _mysql_proxy->free_result(_get_message_stmt);
        return res;
    }
}
