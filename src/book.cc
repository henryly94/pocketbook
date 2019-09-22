#include "book.h"

#include <iostream>
#include <fstream>

#include "absl/strings/str_split.h"

namespace yypocketbook {
Book::Book(const std::vector<EntityFeature>& entity_features) {
    for (const auto& entity_feature : entity_features) {
        AddEntity(entity_feature.first, entity_feature.second);
    }
}

void Book::AddEntity(Entity::Type entity_type, const std::string& entity_name, double amount) {
    if (entity_map_.contains(entity_name)) {
        std::cout << "Account already exist: " << entity_name << std::endl;
        return;
    }
    entity_map_.try_emplace(entity_name, Entity{entity_name, entity_type, amount});
}

void Book::AddTransaction(double amount, absl::string_view from_account_name, const std::string& to_account_name, absl::string_view detail, absl::Time timestamp) {
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
    auto& from_account = from_account_it->second;
    auto& to_account = to_account_it->second;
    Transaction transaction;
    transaction.amount = amount;
    transaction.detail = detail.data();
    transaction.timestamp = timestamp;
    transaction.type = Transaction::CREDIT;
    from_account.AddTransaction(transaction);
    transaction.type = Transaction::DEBIT; 
    to_account.AddTransaction(transaction);
}

void Book::LoadTSV(absl::string_view file_path) {
    std::ifstream in(file_path.data());
    std::string line;
    while (std::getline(in, line)) {
        std::vector<std::string> columns = absl::StrSplit(line, '\t');
        AddEntity(Entity::string_to_type_.at(columns[1]), columns[0], stod(columns[2]));
    }
}

void Book::Print() const {
    for (const auto& entity : entity_map_) {
        entity.second.Print();
    }
}

void Book::PrintWithFilter(EntityFilter filter) const {
    for (const auto& entity : entity_map_) {
        if (filter(entity.second)) {
            entity.second.Print();
        }
    }
}

} // namespace yypocketbook
