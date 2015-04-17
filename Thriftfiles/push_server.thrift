namespace cpp tis
namespace php push
namespace py  push

enum NotifyType {
    INDEX=1, //app首页
    WAP=2, //某个指定的wap页面, 需要给出wap url
    COMMUNITY_DETAIL=3, //贴子详情页, 需要给出贴子tid
    FRIEND=4, //新的好友列表页
    PRIVATE_MSG=5,//私信详情页，需要给出私信的发起人的uid
    SYSTEM_MSG=6 //跳转到系统通知列表页
}

enum InfoType {
    INDEX=1,
    COMMUNITY=2,
    PRIVATE_MSG=4,
    NEW_FRIEND=5,
    MY_MSG=6,
}

enum DeviceType {
    ANDROID=1,
    IOS=2,
}

struct Notify {
    1: required i32 type;
    2: required string content;
    3: required string title='';
    4: required string url='';
    5: required i32 tid=0;
    6: required i32 uid=0;
}

struct Info {
    1: required i32 type;
    2: required string device_id;
    3: required i32 num=-1;
    4: required i32 uid=0;
}

struct SingleNotifyRequest {
    1: required string device_id;
    2: required Notify notify;
    3: required i32 device_type;
}

struct BatchNotifyRequest {
    1: required list<string> device_id_list;
    2: required Notify notify;
    3: required i32 device_type;
    4: required i32 push_task_id=0;
    5: required i32 send_time=1426897707;
}

struct BatchInfoRequest {
    2: required list<Info> info_list;
    3: required i32 device_type;
}

struct BroadcastRequest {
    1: required Notify notify;
    3: required i32 send_time=1426897707;
    4: required i32 push_task_id=0;
}

struct AdminNotifyRequest {
    1: required i16 type;//1:私信,2:关注
    2: required i16 flow;//信息流方向,0:小秘书回复用户,1:用户回复小秘书
    3: required i32 from_uid;
    4: required i32 to_uid;
    5: required i32 ctime;//用户与小秘书交互的时间
    6: required i32 mid; //type为1时的私信id
}

exception InvalidParamException {
    1: string errmsg,
    2: i32 errcode

}
    
service PushService {
    void ping(),
    void single_notify(1:SingleNotifyRequest request) throws (1:InvalidParamException e),
    void batch_notify(1:BatchNotifyRequest request) throws (1:InvalidParamException e),
    void admin_notify(1:AdminNotifyRequest request) throws (1:InvalidParamException e),
    void batch_info(1:BatchInfoRequest request) throws (1:InvalidParamException e),
    string broadcast(1:BroadcastRequest request) throws (1:InvalidParamException e),
    //void multicast(1:Notify notify, 2:i32 gid, 3:i32 begin_time, 4:i32 end_time) throws (1:InvalidParamException e),
    oneway void zip()
}
