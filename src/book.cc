#include "book.h"

#include <fstream>
#include <iostream>

#include "absl/strings/str_split.h"
#include "absl/time/time.h"

namespace yypocketbook {
Book::Book(const std::vector<EntityFeature> &entity_features) {
  for (const auto &entity_feature : entity_features) {
    AddEntity(entity_feature.first, entity_feature.second);
  }
}

void Book::AddEntity(Entity::Type entity_type, const std::string &entity_name,
                     double amount) {
  if (entity_map_.contains(entity_name)) {
    std::cout << "Account already exist: " << entity_name << std::endl;
    return;
  }
  entity_map_.try_emplace(entity_name,
                          Entity{entity_name, entity_type, amount});
}

void Book::AddTransaction(double amount, absl::string_view from_account_name,
                          const std::string &to_account_name,
                          absl::string_view detail, absl::Time timestamp) {
  auto from_account_it = entity_map_.find(from_account_name);
  auto to_account_it = entity_map_.find(to_account_name);
  if (from_account_it == entity_map_.end()) {
    std::cerr << "Not valid operands: " << from_account_name << std::endl;
    return;
  }
  if (to_account_it == entity_map_.end()) {
    std::cerr << "Not valid operands: " << to_account_name << std::endl;
    return;
  }
  auto &from_account = from_account_it->second;
  auto &to_account = to_account_it->second;
  Transaction transaction;
  transaction.amount = amount;
  transaction.detail = detail.data();
  transaction.timestamp = timestamp;
  transaction.type = Transaction::CREDIT;
  from_account.AddTransaction(transaction);
  transaction.type = Transaction::DEBIT;
  to_account.AddTransaction(transaction);
}

void Book::LoadTSVConfig(absl::string_view file_path) {
  std::ifstream in(file_path.data());
  std::string line;
  while (std::getline(in, line)) {
    std::vector<std::string> columns = absl::StrSplit(line, '\t');
    AddEntity(Entity::string_to_type_.at(columns[1]), columns[0],
              stod(columns[2]));
  }
}

void Book::DumpToDisk(absl::string_view file_path) {
  std::ofstream out(file_path.data());
  for (const auto &name_and_entity : entity_map_) {
    const auto &name = name_and_entity.first;
    const auto &entity = name_and_entity.second;
    out << name << '\t' << entity.type_string_[entity.GetType()] << '\t'
        << entity.GetAmount();
    for (const auto &transaction : entity.GetHistory()) {
      out << '\t' << transaction.type << '\t' << transaction.amount << '\t'
          << transaction.timestamp << '\t' << transaction.detail;
    }
    out << '\n';
  }
}

void Book::LoadFromDisk(absl::string_view file_path) {
  std::ifstream in(file_path.data());
  std::string line;
  while (std::getline(in, line)) {
    std::vector<std::string> tokens = absl::StrSplit(line, '\t');
    std::vector<Transaction> transaction_history;
    std::string name{tokens[0]};
    Entity::Type type = Entity::string_to_type_.at(tokens[1]);
    double amount = std::stod(tokens[2]);
    for (int i = 3; i < tokens.size(); i += 4) {
      Transaction transaction;
      transaction.type =
          std::stoi(tokens[i]) ? Transaction::DEBIT : Transaction::CREDIT;
      transaction.amount = std::stod(tokens[i + 1]);
      absl::ParseTime(absl::RFC3339_full, tokens[i + 2], &transaction.timestamp,
                      nullptr);
      transaction.detail = std::move(tokens[i + 3]);
      transaction_history.push_back(std::move(transaction));
    }
    entity_map_[name] = Entity{name, type, amount, transaction_history};
  }
}

void Book::Print() const {
  for (const auto &entity : entity_map_) {
    entity.second.Print();
  }
}

void Book::PrintWithFilter(EntityFilter filter) const {
  for (const auto &entity : entity_map_) {
    if (filter(entity.second)) {
      entity.second.Print();
    }
  }
}

} // namespace yypocketbook
