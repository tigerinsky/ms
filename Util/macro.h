 
#ifndef  __MACRO_H_
#define  __MACRO_H_

#include <string>
#include "macro.h"


struct redisContext;
struct redisReply;

namespace lj {

#define    DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&);                \
        TypeName& operator=(const TypeName&)

}


#endif  //__MACRO_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
