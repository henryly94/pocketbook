#ifndef UTIL_H_
#define UTIL_H_

#include "messages.pb.h"
#include "book.h"

namespace yypocketbook {

protobuf::Book MakeProtoBufBook(Book& book);

} // namespace yypocketbook

#endif // UTIL_H_
