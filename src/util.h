#ifndef UTIL_H_
#define UTIL_H_

#include "absl/container/flat_hash_map.h"
#include "book.h"
#include "messages.pb.h"

namespace yypocketbook {

class Util {
public:
  static protobuf::Book MakeProtobufBook(const Book &book);

  static protobuf::Entity MakeProtobufEntity(const Entity &entity);

  static protobuf::Transaction
  MakeProtobufTransaction(const Transaction &transaction);

  static Book MakeBook(const protobuf::Book &proto_book);

  static Entity MakeEntity(const protobuf::Entity &proto_entity);

  static Transaction
  MakeTransaction(const protobuf::Transaction &proto_transaction);

private:
  const static protobuf::Entity_Type get_protobuf_entity_type_[];

  const static protobuf::Transaction_Type get_protobuf_transaction_type_[];

  const static absl::flat_hash_map<protobuf::Entity_Type, Entity::Type>
      get_entity_type_;

  const static absl::flat_hash_map<protobuf::Transaction_Type,
                                   Transaction::Type>
      get_transaction_type_;
};

} // namespace yypocketbook

#endif // UTIL_H_
