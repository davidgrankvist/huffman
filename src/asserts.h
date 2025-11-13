#ifndef asserts_h
#define asserts_h

#include <assert.h>

#define Assert(b, msg) assert((b) && msg)
#define AssertFail(msg) assert(false && msg)
#define AssertTodo() assert(false && "TODO")

#endif