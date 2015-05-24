namespace cpp tis
namespace php ms
namespace py ms

/**
 * @brief 系统消息类型
 */
enum ActionType {
    AT = 0,             //@
    MAIL = 1,           //私信
    COMMENT = 2,        //评论
    COMMENT_REPLY = 3,  //回复评论
    COMMENT_DELETE = 4, //删除评论
    FOLLOW = 5,         //关注
    PRAISE = 6,         //点赞
    NEW_TWEET = 7,      //小秘书将讨论推荐到要闻
    NEW_FRIEND = 8,     //好友推荐, 新的可关注好友
    MIS_AUTHENTED = 9,  //mis认证通过
}

/**
 * @brief 系统动作消息  
 */
struct SystemMessage {
    1: required i32 from_uid,      //操作者id
    2: required ActionType action_type,   //消息类型, 见上面的ActionType说明
    3: required list<i32> to_uid,        //告知者用户id
    4: required i64 content_id = 0,//动作相关的id, such as tweet id, comment id, 没有则默认为0,如关注
}

enum TweetType {
    INDEX = 1,       //新帖属于首页要闻
    COMMUNITY = 2,   //新帖属于讨论区
}

/*
 * 用户建新帖或者mis推新帖消息
 */
struct NewTweetNotifyRequest {
    1: required list<i32> uids, //需要告知的用户列表
    2: required TweetType type, //帖子类型, 属于首页要闻或讨论区
    3: optional i64 tid,        //帖子id
}

/**
 * @brief mis站外通知
 */
struct MisRequest {
    1: required string title='',   //通知标题
    2: required string content, //通知内容
    4: required i32 type, //推送类型,1:app首页,2:指定的wap页面,3:帖子详情页,4:新的好友列表页,5:私信详情页,6:跳转到系统通知列表页
    5: required i64 tid=0,        //与此通知相关的帖子id,可选
    6: required string url='', //与此通知相关的wap url
    7: required i32 send_time=0, //推送时间
    8: required i32 push_task_id=0, //推送任务id
    9: required i32 device_type=0,//0:全平台 1:android 2:ios
    10: required string city,//城市名称，用逗号分隔，如果是全部城市，请只传字符串"all_city"
    11: required string school, //学校名称，用逗号分隔,如果是全部学校，请只传字符串"all_school"
    12: required string ukind_verify, //取两个值，verify，unverify
    13: required i32 is_broadcast,//是否是广播，1是广播，0不是。广播表示给所有设备推送
}

struct NewFriendRequest {
    1: required i32 to_uid,
    2: required list<i32> rec_uids,//推荐的好友列表
}

struct OpTagRequest {
    1: required i32 uid=0;//用户id
    2: required string xg_device_token='';
    3: required i32 op; //op=1:增加tag, op=2:删除tag
    4: required list<string> tag_list; //需要增加或者删除的tag 列表
}

service MessageServer {

    //系统消息
    void send_system_msg(1:SystemMessage sMsg),

    //设置系统消息为已读
    i32 set_read(1:i32 sMsgId),

    //设置系统消息为已删除
    i32 set_delete(1:i32 sMsgId),

    /* @brief 某个用户读取了首页或帖子区的消息
     * @param uid, 用户id
     *        mType, 用户点击小红点的区域, 1:首页, 2:贴子区, 3:我的消息
     *        num, 用户点击时显示的数字, 如果只显示小红点,则可以不传 
     *        from_uid, 来自谁的私信小红点取消,对方的uid
     */
    i32 clear_red_by_uid(1:i32 uid, 2:i32 mType, 3:i32 num = 0, 4:i32 from_uid=0),

    //新帖发布消息
    void new_tweet_notify(1:NewTweetNotifyRequest request),

    //mis通知消息
    void mis_notify(1:MisRequest request),

    //好友推荐通知
    //void new_friend_notify(1:NewFriendRequest request),

    i32 update_config(1:i32 key, 2:string value),

    //获取1：私信，2：新的好友消息， 3：系统消息的未读数量 
    i32 get_num(1:i32 uid, 2:i32 queue_type),

    i32 optag(1:OpTagRequest request),
}
