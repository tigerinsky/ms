/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "push_server_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace tis {

int _kNotifyTypeValues[] = {
  NotifyType::INDEX,
  NotifyType::WAP,
  NotifyType::COMMUNITY_DETAIL,
  NotifyType::FRIEND,
  NotifyType::PRIVATE_MSG,
  NotifyType::SYSTEM_MSG
};
const char* _kNotifyTypeNames[] = {
  "INDEX",
  "WAP",
  "COMMUNITY_DETAIL",
  "FRIEND",
  "PRIVATE_MSG",
  "SYSTEM_MSG"
};
const std::map<int, const char*> _NotifyType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(6, _kNotifyTypeValues, _kNotifyTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kInfoTypeValues[] = {
  InfoType::INDEX,
  InfoType::COMMUNITY,
  InfoType::PRIVATE_MSG,
  InfoType::NEW_FRIEND,
  InfoType::MY_MSG
};
const char* _kInfoTypeNames[] = {
  "INDEX",
  "COMMUNITY",
  "PRIVATE_MSG",
  "NEW_FRIEND",
  "MY_MSG"
};
const std::map<int, const char*> _InfoType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(5, _kInfoTypeValues, _kInfoTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kDeviceTypeValues[] = {
  DeviceType::ANDROID,
  DeviceType::IOS
};
const char* _kDeviceTypeNames[] = {
  "ANDROID",
  "IOS"
};
const std::map<int, const char*> _DeviceType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(2, _kDeviceTypeValues, _kDeviceTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));


Notify::~Notify() throw() {
}


void Notify::__set_type(const int32_t val) {
  this->type = val;
}

void Notify::__set_content(const std::string& val) {
  this->content = val;
}

void Notify::__set_title(const std::string& val) {
  this->title = val;
}

void Notify::__set_url(const std::string& val) {
  this->url = val;
}

void Notify::__set_tid(const int32_t val) {
  this->tid = val;
}

void Notify::__set_uid(const int32_t val) {
  this->uid = val;
}

const char* Notify::ascii_fingerprint = "54E7F89931576CA0EA7FDD572B1F6F70";
const uint8_t Notify::binary_fingerprint[16] = {0x54,0xE7,0xF8,0x99,0x31,0x57,0x6C,0xA0,0xEA,0x7F,0xDD,0x57,0x2B,0x1F,0x6F,0x70};

uint32_t Notify::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_type = false;
  bool isset_content = false;
  bool isset_title = false;
  bool isset_url = false;
  bool isset_tid = false;
  bool isset_uid = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->type);
          isset_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->content);
          isset_content = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->title);
          isset_title = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->url);
          isset_url = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->tid);
          isset_tid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->uid);
          isset_uid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_content)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_title)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_url)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_tid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_uid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Notify::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("Notify");

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("content", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->content);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("title", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->title);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("url", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->url);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("tid", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->tid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("uid", ::apache::thrift::protocol::T_I32, 6);
  xfer += oprot->writeI32(this->uid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(Notify &a, Notify &b) {
  using ::std::swap;
  swap(a.type, b.type);
  swap(a.content, b.content);
  swap(a.title, b.title);
  swap(a.url, b.url);
  swap(a.tid, b.tid);
  swap(a.uid, b.uid);
}

Notify::Notify(const Notify& other0) {
  type = other0.type;
  content = other0.content;
  title = other0.title;
  url = other0.url;
  tid = other0.tid;
  uid = other0.uid;
}
Notify& Notify::operator=(const Notify& other1) {
  type = other1.type;
  content = other1.content;
  title = other1.title;
  url = other1.url;
  tid = other1.tid;
  uid = other1.uid;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const Notify& obj) {
  using apache::thrift::to_string;
  out << "Notify(";
  out << "type=" << to_string(obj.type);
  out << ", " << "content=" << to_string(obj.content);
  out << ", " << "title=" << to_string(obj.title);
  out << ", " << "url=" << to_string(obj.url);
  out << ", " << "tid=" << to_string(obj.tid);
  out << ", " << "uid=" << to_string(obj.uid);
  out << ")";
  return out;
}


Info::~Info() throw() {
}


void Info::__set_type(const int32_t val) {
  this->type = val;
}

void Info::__set_device_id(const std::string& val) {
  this->device_id = val;
}

void Info::__set_num(const int32_t val) {
  this->num = val;
}

void Info::__set_uid(const int32_t val) {
  this->uid = val;
}

const char* Info::ascii_fingerprint = "AD5E8581BBE4C3CDE5FC5930DA3DD601";
const uint8_t Info::binary_fingerprint[16] = {0xAD,0x5E,0x85,0x81,0xBB,0xE4,0xC3,0xCD,0xE5,0xFC,0x59,0x30,0xDA,0x3D,0xD6,0x01};

uint32_t Info::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_type = false;
  bool isset_device_id = false;
  bool isset_num = false;
  bool isset_uid = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->type);
          isset_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->device_id);
          isset_device_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->num);
          isset_num = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->uid);
          isset_uid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_device_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_num)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_uid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Info::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("Info");

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("device_id", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->device_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("num", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->num);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("uid", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->uid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(Info &a, Info &b) {
  using ::std::swap;
  swap(a.type, b.type);
  swap(a.device_id, b.device_id);
  swap(a.num, b.num);
  swap(a.uid, b.uid);
}

Info::Info(const Info& other2) {
  type = other2.type;
  device_id = other2.device_id;
  num = other2.num;
  uid = other2.uid;
}
Info& Info::operator=(const Info& other3) {
  type = other3.type;
  device_id = other3.device_id;
  num = other3.num;
  uid = other3.uid;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const Info& obj) {
  using apache::thrift::to_string;
  out << "Info(";
  out << "type=" << to_string(obj.type);
  out << ", " << "device_id=" << to_string(obj.device_id);
  out << ", " << "num=" << to_string(obj.num);
  out << ", " << "uid=" << to_string(obj.uid);
  out << ")";
  return out;
}


SingleNotifyRequest::~SingleNotifyRequest() throw() {
}


void SingleNotifyRequest::__set_device_id(const std::string& val) {
  this->device_id = val;
}

void SingleNotifyRequest::__set_notify(const Notify& val) {
  this->notify = val;
}

void SingleNotifyRequest::__set_device_type(const int32_t val) {
  this->device_type = val;
}

const char* SingleNotifyRequest::ascii_fingerprint = "8BEDB189560815C23D9C74B0F45DFFA0";
const uint8_t SingleNotifyRequest::binary_fingerprint[16] = {0x8B,0xED,0xB1,0x89,0x56,0x08,0x15,0xC2,0x3D,0x9C,0x74,0xB0,0xF4,0x5D,0xFF,0xA0};

uint32_t SingleNotifyRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_device_id = false;
  bool isset_notify = false;
  bool isset_device_type = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->device_id);
          isset_device_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->notify.read(iprot);
          isset_notify = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->device_type);
          isset_device_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_device_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_notify)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_device_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t SingleNotifyRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("SingleNotifyRequest");

  xfer += oprot->writeFieldBegin("device_id", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->device_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("notify", ::apache::thrift::protocol::T_STRUCT, 2);
  xfer += this->notify.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("device_type", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->device_type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(SingleNotifyRequest &a, SingleNotifyRequest &b) {
  using ::std::swap;
  swap(a.device_id, b.device_id);
  swap(a.notify, b.notify);
  swap(a.device_type, b.device_type);
}

SingleNotifyRequest::SingleNotifyRequest(const SingleNotifyRequest& other4) {
  device_id = other4.device_id;
  notify = other4.notify;
  device_type = other4.device_type;
}
SingleNotifyRequest& SingleNotifyRequest::operator=(const SingleNotifyRequest& other5) {
  device_id = other5.device_id;
  notify = other5.notify;
  device_type = other5.device_type;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const SingleNotifyRequest& obj) {
  using apache::thrift::to_string;
  out << "SingleNotifyRequest(";
  out << "device_id=" << to_string(obj.device_id);
  out << ", " << "notify=" << to_string(obj.notify);
  out << ", " << "device_type=" << to_string(obj.device_type);
  out << ")";
  return out;
}


BatchNotifyRequest::~BatchNotifyRequest() throw() {
}


void BatchNotifyRequest::__set_device_id_list(const std::vector<std::string> & val) {
  this->device_id_list = val;
}

void BatchNotifyRequest::__set_notify(const Notify& val) {
  this->notify = val;
}

void BatchNotifyRequest::__set_device_type(const int32_t val) {
  this->device_type = val;
}

void BatchNotifyRequest::__set_push_task_id(const int32_t val) {
  this->push_task_id = val;
}

void BatchNotifyRequest::__set_send_time(const int32_t val) {
  this->send_time = val;
}

const char* BatchNotifyRequest::ascii_fingerprint = "2F550C4B5CB27788B4770238355FE5A2";
const uint8_t BatchNotifyRequest::binary_fingerprint[16] = {0x2F,0x55,0x0C,0x4B,0x5C,0xB2,0x77,0x88,0xB4,0x77,0x02,0x38,0x35,0x5F,0xE5,0xA2};

uint32_t BatchNotifyRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_device_id_list = false;
  bool isset_notify = false;
  bool isset_device_type = false;
  bool isset_push_task_id = false;
  bool isset_send_time = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->device_id_list.clear();
            uint32_t _size6;
            ::apache::thrift::protocol::TType _etype9;
            xfer += iprot->readListBegin(_etype9, _size6);
            this->device_id_list.resize(_size6);
            uint32_t _i10;
            for (_i10 = 0; _i10 < _size6; ++_i10)
            {
              xfer += iprot->readString(this->device_id_list[_i10]);
            }
            xfer += iprot->readListEnd();
          }
          isset_device_id_list = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->notify.read(iprot);
          isset_notify = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->device_type);
          isset_device_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->push_task_id);
          isset_push_task_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->send_time);
          isset_send_time = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_device_id_list)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_notify)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_device_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_push_task_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_send_time)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t BatchNotifyRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("BatchNotifyRequest");

  xfer += oprot->writeFieldBegin("device_id_list", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->device_id_list.size()));
    std::vector<std::string> ::const_iterator _iter11;
    for (_iter11 = this->device_id_list.begin(); _iter11 != this->device_id_list.end(); ++_iter11)
    {
      xfer += oprot->writeString((*_iter11));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("notify", ::apache::thrift::protocol::T_STRUCT, 2);
  xfer += this->notify.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("device_type", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->device_type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("push_task_id", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->push_task_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("send_time", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->send_time);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(BatchNotifyRequest &a, BatchNotifyRequest &b) {
  using ::std::swap;
  swap(a.device_id_list, b.device_id_list);
  swap(a.notify, b.notify);
  swap(a.device_type, b.device_type);
  swap(a.push_task_id, b.push_task_id);
  swap(a.send_time, b.send_time);
}

BatchNotifyRequest::BatchNotifyRequest(const BatchNotifyRequest& other12) {
  device_id_list = other12.device_id_list;
  notify = other12.notify;
  device_type = other12.device_type;
  push_task_id = other12.push_task_id;
  send_time = other12.send_time;
}
BatchNotifyRequest& BatchNotifyRequest::operator=(const BatchNotifyRequest& other13) {
  device_id_list = other13.device_id_list;
  notify = other13.notify;
  device_type = other13.device_type;
  push_task_id = other13.push_task_id;
  send_time = other13.send_time;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const BatchNotifyRequest& obj) {
  using apache::thrift::to_string;
  out << "BatchNotifyRequest(";
  out << "device_id_list=" << to_string(obj.device_id_list);
  out << ", " << "notify=" << to_string(obj.notify);
  out << ", " << "device_type=" << to_string(obj.device_type);
  out << ", " << "push_task_id=" << to_string(obj.push_task_id);
  out << ", " << "send_time=" << to_string(obj.send_time);
  out << ")";
  return out;
}


BatchInfoRequest::~BatchInfoRequest() throw() {
}


void BatchInfoRequest::__set_info_list(const std::vector<Info> & val) {
  this->info_list = val;
}

void BatchInfoRequest::__set_device_type(const int32_t val) {
  this->device_type = val;
}

const char* BatchInfoRequest::ascii_fingerprint = "391C28D3226FBD3FCA1AB6FEE7917575";
const uint8_t BatchInfoRequest::binary_fingerprint[16] = {0x39,0x1C,0x28,0xD3,0x22,0x6F,0xBD,0x3F,0xCA,0x1A,0xB6,0xFE,0xE7,0x91,0x75,0x75};

uint32_t BatchInfoRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_info_list = false;
  bool isset_device_type = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->info_list.clear();
            uint32_t _size14;
            ::apache::thrift::protocol::TType _etype17;
            xfer += iprot->readListBegin(_etype17, _size14);
            this->info_list.resize(_size14);
            uint32_t _i18;
            for (_i18 = 0; _i18 < _size14; ++_i18)
            {
              xfer += this->info_list[_i18].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_info_list = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->device_type);
          isset_device_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_info_list)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_device_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t BatchInfoRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("BatchInfoRequest");

  xfer += oprot->writeFieldBegin("info_list", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->info_list.size()));
    std::vector<Info> ::const_iterator _iter19;
    for (_iter19 = this->info_list.begin(); _iter19 != this->info_list.end(); ++_iter19)
    {
      xfer += (*_iter19).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("device_type", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->device_type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(BatchInfoRequest &a, BatchInfoRequest &b) {
  using ::std::swap;
  swap(a.info_list, b.info_list);
  swap(a.device_type, b.device_type);
}

BatchInfoRequest::BatchInfoRequest(const BatchInfoRequest& other20) {
  info_list = other20.info_list;
  device_type = other20.device_type;
}
BatchInfoRequest& BatchInfoRequest::operator=(const BatchInfoRequest& other21) {
  info_list = other21.info_list;
  device_type = other21.device_type;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const BatchInfoRequest& obj) {
  using apache::thrift::to_string;
  out << "BatchInfoRequest(";
  out << "info_list=" << to_string(obj.info_list);
  out << ", " << "device_type=" << to_string(obj.device_type);
  out << ")";
  return out;
}


BroadcastRequest::~BroadcastRequest() throw() {
}


void BroadcastRequest::__set_notify(const Notify& val) {
  this->notify = val;
}

void BroadcastRequest::__set_send_time(const int32_t val) {
  this->send_time = val;
}

void BroadcastRequest::__set_push_task_id(const int32_t val) {
  this->push_task_id = val;
}

const char* BroadcastRequest::ascii_fingerprint = "DF0CAEBB6F2D1BB128517CCCE5C215BF";
const uint8_t BroadcastRequest::binary_fingerprint[16] = {0xDF,0x0C,0xAE,0xBB,0x6F,0x2D,0x1B,0xB1,0x28,0x51,0x7C,0xCC,0xE5,0xC2,0x15,0xBF};

uint32_t BroadcastRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_notify = false;
  bool isset_send_time = false;
  bool isset_push_task_id = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->notify.read(iprot);
          isset_notify = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->send_time);
          isset_send_time = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->push_task_id);
          isset_push_task_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_notify)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_send_time)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_push_task_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t BroadcastRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("BroadcastRequest");

  xfer += oprot->writeFieldBegin("notify", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->notify.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("send_time", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->send_time);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("push_task_id", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->push_task_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(BroadcastRequest &a, BroadcastRequest &b) {
  using ::std::swap;
  swap(a.notify, b.notify);
  swap(a.send_time, b.send_time);
  swap(a.push_task_id, b.push_task_id);
}

BroadcastRequest::BroadcastRequest(const BroadcastRequest& other22) {
  notify = other22.notify;
  send_time = other22.send_time;
  push_task_id = other22.push_task_id;
}
BroadcastRequest& BroadcastRequest::operator=(const BroadcastRequest& other23) {
  notify = other23.notify;
  send_time = other23.send_time;
  push_task_id = other23.push_task_id;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const BroadcastRequest& obj) {
  using apache::thrift::to_string;
  out << "BroadcastRequest(";
  out << "notify=" << to_string(obj.notify);
  out << ", " << "send_time=" << to_string(obj.send_time);
  out << ", " << "push_task_id=" << to_string(obj.push_task_id);
  out << ")";
  return out;
}


AdminNotifyRequest::~AdminNotifyRequest() throw() {
}


void AdminNotifyRequest::__set_type(const int16_t val) {
  this->type = val;
}

void AdminNotifyRequest::__set_flow(const int16_t val) {
  this->flow = val;
}

void AdminNotifyRequest::__set_from_uid(const int32_t val) {
  this->from_uid = val;
}

void AdminNotifyRequest::__set_to_uid(const int32_t val) {
  this->to_uid = val;
}

void AdminNotifyRequest::__set_ctime(const int32_t val) {
  this->ctime = val;
}

void AdminNotifyRequest::__set_mid(const int32_t val) {
  this->mid = val;
}

const char* AdminNotifyRequest::ascii_fingerprint = "25CA238F7F8E4D993802165C2172B80F";
const uint8_t AdminNotifyRequest::binary_fingerprint[16] = {0x25,0xCA,0x23,0x8F,0x7F,0x8E,0x4D,0x99,0x38,0x02,0x16,0x5C,0x21,0x72,0xB8,0x0F};

uint32_t AdminNotifyRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_type = false;
  bool isset_flow = false;
  bool isset_from_uid = false;
  bool isset_to_uid = false;
  bool isset_ctime = false;
  bool isset_mid = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I16) {
          xfer += iprot->readI16(this->type);
          isset_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I16) {
          xfer += iprot->readI16(this->flow);
          isset_flow = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->from_uid);
          isset_from_uid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->to_uid);
          isset_to_uid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->ctime);
          isset_ctime = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->mid);
          isset_mid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_flow)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_from_uid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_to_uid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_ctime)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_mid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t AdminNotifyRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("AdminNotifyRequest");

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_I16, 1);
  xfer += oprot->writeI16(this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("flow", ::apache::thrift::protocol::T_I16, 2);
  xfer += oprot->writeI16(this->flow);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("from_uid", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->from_uid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("to_uid", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->to_uid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ctime", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->ctime);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("mid", ::apache::thrift::protocol::T_I32, 6);
  xfer += oprot->writeI32(this->mid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(AdminNotifyRequest &a, AdminNotifyRequest &b) {
  using ::std::swap;
  swap(a.type, b.type);
  swap(a.flow, b.flow);
  swap(a.from_uid, b.from_uid);
  swap(a.to_uid, b.to_uid);
  swap(a.ctime, b.ctime);
  swap(a.mid, b.mid);
}

AdminNotifyRequest::AdminNotifyRequest(const AdminNotifyRequest& other24) {
  type = other24.type;
  flow = other24.flow;
  from_uid = other24.from_uid;
  to_uid = other24.to_uid;
  ctime = other24.ctime;
  mid = other24.mid;
}
AdminNotifyRequest& AdminNotifyRequest::operator=(const AdminNotifyRequest& other25) {
  type = other25.type;
  flow = other25.flow;
  from_uid = other25.from_uid;
  to_uid = other25.to_uid;
  ctime = other25.ctime;
  mid = other25.mid;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const AdminNotifyRequest& obj) {
  using apache::thrift::to_string;
  out << "AdminNotifyRequest(";
  out << "type=" << to_string(obj.type);
  out << ", " << "flow=" << to_string(obj.flow);
  out << ", " << "from_uid=" << to_string(obj.from_uid);
  out << ", " << "to_uid=" << to_string(obj.to_uid);
  out << ", " << "ctime=" << to_string(obj.ctime);
  out << ", " << "mid=" << to_string(obj.mid);
  out << ")";
  return out;
}


InvalidParamException::~InvalidParamException() throw() {
}


void InvalidParamException::__set_errmsg(const std::string& val) {
  this->errmsg = val;
}

void InvalidParamException::__set_errcode(const int32_t val) {
  this->errcode = val;
}

const char* InvalidParamException::ascii_fingerprint = "EEBC915CE44901401D881E6091423036";
const uint8_t InvalidParamException::binary_fingerprint[16] = {0xEE,0xBC,0x91,0x5C,0xE4,0x49,0x01,0x40,0x1D,0x88,0x1E,0x60,0x91,0x42,0x30,0x36};

uint32_t InvalidParamException::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->errmsg);
          this->__isset.errmsg = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->errcode);
          this->__isset.errcode = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t InvalidParamException::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("InvalidParamException");

  xfer += oprot->writeFieldBegin("errmsg", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->errmsg);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("errcode", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->errcode);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(InvalidParamException &a, InvalidParamException &b) {
  using ::std::swap;
  swap(a.errmsg, b.errmsg);
  swap(a.errcode, b.errcode);
  swap(a.__isset, b.__isset);
}

InvalidParamException::InvalidParamException(const InvalidParamException& other26) : TException() {
  errmsg = other26.errmsg;
  errcode = other26.errcode;
  __isset = other26.__isset;
}
InvalidParamException& InvalidParamException::operator=(const InvalidParamException& other27) {
  errmsg = other27.errmsg;
  errcode = other27.errcode;
  __isset = other27.__isset;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const InvalidParamException& obj) {
  using apache::thrift::to_string;
  out << "InvalidParamException(";
  out << "errmsg=" << to_string(obj.errmsg);
  out << ", " << "errcode=" << to_string(obj.errcode);
  out << ")";
  return out;
}

} // namespace