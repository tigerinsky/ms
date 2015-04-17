// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <signal.h>
#include <pthread.h>
#include "glog/logging.h"
#include "mysql_proxy.h"

#include "ms_flag.h"
#include "Handler/message_server_handler.h"
#include "Handler/red_remind.h"
#include "Util/constant.h"

namespace tis {

DEFINE_int32(server_port, 8999, "ms server port");
DEFINE_int32(server_thread_num, 15, "thread pool server handler thread num");
DEFINE_string(db_redis_host, "127.0.0.1", "db redis host");
DEFINE_int32(db_redis_port, 8888, "db redis post");
DEFINE_string(cache_redis_host, "127.0.0.1", "cache redis host");
DEFINE_int32(cache_redis_port, 8889, "cache redis port");
DEFINE_int32(thread_sleep_time, 10000, "task handler thread sleep time");
DEFINE_string(ps_host, "127.0.0.1", "push server host");
DEFINE_int32(ps_port, 8998, "push server port");
DEFINE_string(mysql_host, "127.0.0.1", "mysql server host");
DEFINE_int32(mysql_port, 3306, "mysql server port");
DEFINE_string(mysql_db, "", "mysql db");
DEFINE_string(mysql_user, "root", "mysql user");
DEFINE_string(mysql_pwd, "", "mysql password");
DEFINE_int32(day_interval, 59, "red remind day interval");
DEFINE_int32(night_interval, 1879, "red remind night interval");
DEFINE_int32(msg_interval, 30, "red remind msg interval");
DEFINE_int32(expired_queue_max_len, 200, "expired queue max len");
DEFINE_int32(expired_queue_rem_step, 20, "expired queue rem step");
DEFINE_int32(expire_time_in_second, 60, "expire time in second");
DEFINE_int32(batch_push_num, 750, "batch push once");

global_data_t g_data;

}

using namespace tis;

void* start_bottom_remind(void *arg){
    RedRemind remind;
    if (0 == remind.init())
        remind.red_remind_bottom();
    return NULL;
}

void* start_msg_remind(void *arg) {
    RedRemind remind;
    if (0 == remind.init())
        remind.red_remind_msg();
    return NULL;
}

void handle_signal(int sig) {
    LOG(INFO) << "why kill me!" << endl;
    g_data.g_ms_server.stop();
    pthread_cancel(g_data.g_pid);
    pthread_cancel(g_data.g_pid2);
    LOG(INFO) << "kill over!" << endl;
    exit(0);
}

int main(int argc, char **argv) {
    ::google::ParseCommandLineFlags(&argc, &argv, false);
    ::google::SetUsageMessage("message server");

    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGPIPE, SIG_IGN);

    g_data.g_shared_user_info.init();
    g_data.g_shared_push_config.init();
    //pthread_t pid;
    pthread_create(&(g_data.g_pid), NULL, start_bottom_remind, NULL);
    pthread_create(&(g_data.g_pid2), NULL, start_msg_remind, NULL);

    if (0 != g_data.g_ms_server.init()) {
        LOG(WARNING) << "init ms server wrong!" << endl;
        return 0;
    }

    if (0 != g_data.g_ms_server.run()) {
        LOG(WARNING) << "run ms server error!" << endl;
        return 0;
    }

    pthread_join(g_data.g_pid, NULL);
    pthread_join(g_data.g_pid2, NULL);
    return 0;
}
