#ifndef __MESSAGE_SERVER_HANDLER_H_
#define __MESSAGE_SERVER_HANDLER_H_

#include <vector>
#include <string>
#include <map>
#include "Thriftfiles/MessageServer.h"

using std::string;
using std::vector;
using namespace ::apache::thrift;

namespace tis {

#define TIMEDIFF(s, e)  ((e.tv_sec-s.tv_sec)*1000000 + e.tv_usec - s.tv_usec)

class RedisProxy;
class PushClient;
class MysqlDAO;

class MessageServerHandler : virtual public MessageServerIf {
 public:
     static int init_thread_data(int thread_num);
 public:
  MessageServerHandler(){
  }

  ~MessageServerHandler() {
  }

  void send_system_msg(const SystemMessage& sMsg);

  int32_t set_read(const int32_t sMsgId);

  int32_t set_delete(const int32_t sMsgId);
  
  //消除小红点
  int32_t clear_red_by_uid(
          const int32_t uid, 
          const int32_t mType, 
          const int32_t num,
          const int32_t from_uid);

  //有新帖消息
  void new_tweet_notify(const NewTweetNotifyRequest& request);

  //mis通知消息
  void mis_notify(const MisRequest& request);

  //void new_friend_notify(const NewFriendRequest& request);

  int32_t update_config(const int32_t key, const std::string& value);

  int32_t get_num(const int32_t uid, const int32_t queue_type);

  int32_t optag(const OpTagRequest& request);

 private:
  long get_timestamp();

  //从redis中获取用户名称
  int get_username(int32_t uid, std::string& name);

  //从catch redis中获取用户的dummy类型
  int get_dummy(int32_t uid);

  //生成好友推荐的推送文案
  string generate_friend_notify_content(vector<int32_t> uids);

  bool can_push(int32_t uid, string config_type);

  //关注和私信通知蓝鲸小秘书
  /*int admin_notify(int16_t type, int16_t flow,
          int32_t from_uid, int32_t to_uid, 
          int32_t ctime, int32_t mid = 0);*/

  //检查最近是否有点赞或关注,避免频繁推消息
  bool check_action_frequent(int32_t from_uid,
          int type, int32_t to_uid, int64_t content_id);
};

}
#endif
