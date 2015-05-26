#ifndef __MYSQL_DAO_H_
#define __MYSQL_DAO_H_

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace tis {

struct mysql_stmt_t;
class MysqlProxy;
class UserPush;
class UserIndustry;
class PushConfig;
class CiMessage;

class MysqlDAO {

public:
    MysqlDAO() {
    };
    ~MysqlDAO() {
        destroy();
    }
    int init(const char* host, int port,
            const char* user, const char* pwd,
            const char* db);
    void destroy();
    int insert_system_message(int32_t from_uid,
            const char* from_name,
            int32_t action_type,
            int32_t to_uid,
            const char* to_name,
            int64_t content_id,
            long create_time,
            long update_time);
    int set_system_message_read(int32_t sys_message_id,
            long time);
    int set_system_message_delete(int32_t sys_message_id,
            long time);
    void get_industry_users_by_limit(int32_t industry_id,
            vector<long> &uid_list, int offset, int limit);
    int get_device_info(int32_t uid, vector<int32_t> &device_type, 
            vector<string> &xg_device_token);
    
    int get_push_config(int32_t uid, string &config);
    //读取私信
    int get_message(const int32_t mid, string &content, int32_t &ctime);

private:
  MysqlProxy *_mysql_proxy;

  mysql_stmt_t *_send_msg_stmt; 
  mysql_stmt_t *_set_read_stmt;
  mysql_stmt_t *_set_delete_stmt;

  mysql_stmt_t *_get_device_info_stmt;
  UserPush* _user_push;

  mysql_stmt_t *_get_push_config_stmt;
  PushConfig* _push_config;

  mysql_stmt_t *_get_industry_users_stmt;
  UserIndustry *_user_industry;

  mysql_stmt_t *_get_message_stmt;
  CiMessage *_ci_message;
};

}

#endif

