#include "util.h"

#include "entity.h"
#include "absl/time/time.h"

namespace yypocketbook {
namespace {

using ::yypocketbook::protobuf::Entity_Type;
using ::yypocketbook::protobuf::Entity;
using ::yypocketbook::protobuf::Transaction_Type;
using ::yypocketbook::protobuf::Transaction;

constexpr Entity_Type get_protobuf_entity_type[] = {
    Entity::ASSET,
    Entity::EXPENSE,
    Entity::LIABILITY,
    Entity::EQUITY,
    Entity::REVENUE};

constexpr Transaction_Type get_protobuf_transaction_type[] = {
    Transaction::CREDIT,
    Transaction::DEBIT,
};

}  // namespace

protobuf::Book MakeProtoBufBook(Book& book) {
  protobuf::Book proto_book;
  for (const auto& name_entity_pair : book.GetEntityMap()) {
    auto* new_entity = proto_book.add_entities();
    new_entity->set_name(name_entity_pair.first);
    new_entity->set_amount(name_entity_pair.second.GetAmount());
    new_entity->set_type(get_protobuf_entity_type[static_cast<int>(
        name_entity_pair.second.GetType())]);
    for (const auto& transaction : name_entity_pair.second.GetHistory()) {
      auto* new_transaction =
          new_entity->mutable_transaction_history()->add_transactions();
      new_transaction->set_type(
          get_protobuf_transaction_type[transaction.type]);
      new_transaction->set_amount(transaction.amount);
      new_transaction->set_detail(transaction.detail);
      new_transaction->set_time_usec(absl::ToUnixMicros(transaction.timestamp));
    }
  }
  return proto_book;
}

}  // namespace yypocketbook
