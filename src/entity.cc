#include "entity.h"
#include <iostream>

namespace yypocketbook {
    
const std::vector<std::string> Entity::type_string_ ={"Asset", "Expense", "Liability", "Equity", "Revenue"};

const absl::flat_hash_map<std::string, Entity::Type> Entity::string_to_type_ = {
    {"Asset", Entity::ASSET},
    {"Expense", Entity::EXPENSE},
    {"Liability", Entity::LIABILITY},
    {"Revenue", Entity::REVENUE},
    {"Equity", Entity::EQUITY},
};

void Entity::AddTransaction(const Transaction& transaction) {
    if (transaction.type == Transaction::CREDIT) {
        if (type == ASSET || type == EXPENSE) {
            amount -= transaction.amount;
        } else { // LIABILITY, EQUITY, REVENUE
            amount += transaction.amount;
        }
    } else { // DEBIT
        if (type == ASSET || type == EXPENSE) {
            amount += transaction.amount;
        } else { // LIABILITY, EQUITY, REVENUE
            amount -= transaction.amount;
        }
    }
    transaction_history.emplace_back(transaction);  
}

absl::flat_hash_set<Entity::Type>& Entity::LeftsideType() const {
    static absl::flat_hash_set<Entity::Type> leftside_types = {
        ASSET, 
        EXPENSE,
    };
    return leftside_types;
}

void Entity::Print() const {
    std::cout << "Account: " << name << std::endl;
    std::cout << "Type: " << type_string_[type] << std::endl; 
    std::cout << "Amount: " << amount << std::endl;
}
} // namespace yypocketbook
