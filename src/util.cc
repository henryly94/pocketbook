#include "util.h"

#include "absl/time/time.h"
#include "entity.h"

namespace yypocketbook {

const protobuf::Entity_Type Util::get_protobuf_entity_type_[] = {
    protobuf::Entity::ASSET, protobuf::Entity::EXPENSE,
    protobuf::Entity::LIABILITY, protobuf::Entity::EQUITY,
    protobuf::Entity::REVENUE};

const protobuf::Transaction_Type Util::get_protobuf_transaction_type_[] = {
    protobuf::Transaction::CREDIT,
    protobuf::Transaction::DEBIT,
};

const absl::flat_hash_map<protobuf::Entity_Type, Entity::Type>
    Util::get_entity_type_ = {
        {protobuf::Entity::ASSET, Entity::ASSET},
        {protobuf::Entity::EXPENSE, Entity::EXPENSE},
        {protobuf::Entity::LIABILITY, Entity::LIABILITY},
        {protobuf::Entity::EQUITY, Entity::EQUITY},
        {protobuf::Entity::REVENUE, Entity::REVENUE},
};

const absl::flat_hash_map<protobuf::Transaction_Type, Transaction::Type>
    Util::get_transaction_type_ = {
        {protobuf::Transaction::CREDIT, Transaction::CREDIT},
        {protobuf::Transaction::DEBIT, Transaction::DEBIT},
};

protobuf::Book Util::MakeProtobufBook(const Book &book) {
  protobuf::Book proto_book;
  for (const auto &name_entity_pair : book.GetEntityMap()) {
    *proto_book.add_entities() =
        std::move(MakeProtobufEntity(name_entity_pair.second));
  }
  return proto_book;
}

protobuf::Entity Util::MakeProtobufEntity(const Entity &entity) {
  protobuf::Entity proto_entity;
  proto_entity.set_name(entity.GetName());
  proto_entity.set_amount(entity.GetAmount());
  proto_entity.set_type(
      get_protobuf_entity_type_[static_cast<int>(entity.GetType())]);

  for (const auto &transaction : entity.GetHistory()) {
    *proto_entity.mutable_transaction_history()->add_transactions() =
        std::move(MakeProtobufTransaction(transaction));
  }
  return proto_entity;
}

protobuf::Transaction
Util::MakeProtobufTransaction(const Transaction &transaction) {
  protobuf::Transaction proto_transaction;
  proto_transaction.set_type(
      get_protobuf_transaction_type_[static_cast<int>(transaction.type)]);
  proto_transaction.set_detail(transaction.detail);
  proto_transaction.set_amount(transaction.amount);
  proto_transaction.set_time_usec(absl::ToUnixMicros(transaction.timestamp));
  return proto_transaction;
}

Transaction
Util::MakeTransaction(const protobuf::Transaction &proto_transaction) {
  Transaction transaction;
  transaction.type = get_transaction_type_.at(proto_transaction.type());
  transaction.amount = proto_transaction.amount();
  transaction.timestamp = absl::FromUnixMicros(proto_transaction.time_usec());
  transaction.detail = proto_transaction.detail();
  return transaction;
}

Entity Util::MakeEntity(const protobuf::Entity &proto_entity) {
  std::vector<Transaction> transaction_history;
  transaction_history.reserve(
      proto_entity.transaction_history().transactions_size());
  for (const auto &proto_transaction :
       proto_entity.transaction_history().transactions()) {
    transaction_history.emplace_back(MakeTransaction(proto_transaction));
  }
  return Entity{proto_entity.name(), get_entity_type_.at(proto_entity.type()),
                proto_entity.amount(), std::move(transaction_history)};
}

Book Util::MakeBook(const protobuf::Book &proto_book) {
  Book book;
  for (const auto &proto_entity : proto_book.entities()) {
  }
  return book;
}

} // namespace yypocketbook
