#ifndef __CONSTANT_H_
#define __CONSTANT_H_

#include <pthread.h>
#include "../Server/ms_server.h"
#include "shared_hash_map.h"

namespace tis {

typedef struct global_data_t {
    MsServer        g_ms_server;
    SharedHashMap   g_shared_user_info;
    SharedHashMap   g_shared_push_config;
    pthread_t       g_pid;
    pthread_t       g_pid2;
}global_data;

extern global_data_t g_data;

}

#endif
