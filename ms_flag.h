#ifndef  __MS_FLAG_H_ 
#define  __MS_FLAG_H_

#include "google/gflags.h"

namespace tis {

DECLARE_int32(server_port);
DECLARE_int32(server_thread_num);
DECLARE_string(db_redis_host);
DECLARE_int32(db_redis_port);
DECLARE_string(cache_redis_host);
DECLARE_int32(cache_redis_port);
DECLARE_int32(thread_sleep_time);
DECLARE_string(ps_host);
DECLARE_int32(ps_port);
DECLARE_string(mysql_host);
DECLARE_int32(mysql_port);
DECLARE_string(mysql_db);
DECLARE_string(mysql_user);
DECLARE_string(mysql_pwd);
DECLARE_int32(day_interval);
DECLARE_int32(night_interval);
DECLARE_int32(msg_interval);
DECLARE_int32(expired_queue_max_len);
DECLARE_int32(expired_queue_rem_step);
DECLARE_int32(expire_time_in_second);
DECLARE_int32(batch_push_num);

}
#endif
