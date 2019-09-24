#ifndef UTIL_H_
#define UTIL_H_

#include "messages.pb.h"
#include "book.h"

namespace yypocketbook {

protobuf::Book MakeProtoBufBook(Book& book) {
  protobuf::Book proto_book;
  for (const auto& name_entity_pair : book.GetEntityMap()) {
    auto* new_entity = proto_book.add_entities();
    new_entity->set_name(name_entity_pair.first);
  }
  return proto_book;
}

} // namespace yypocketbook

#endif // UTIL_H_
