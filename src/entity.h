#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/time/time.h"

namespace yypocketbook {

struct Transaction {
    enum Type {
        CREDIT,
        DEBIT,
    };
    Type type;
    double amount;
    absl::Time timestamp;
    std::string detail;
};

class Entity {
public:
    enum Type {
        ASSET = 0,
        EXPENSE,
        LIABILITY,
        EQUITY,
        REVENUE,
    };
    
    static const std::vector<std::string> type_string_;
    static const absl::flat_hash_map<std::string, Type> string_to_type_;
    Entity() {}

    explicit Entity(
        std::string name, 
        Type type, 
        double amount=0.0,
        std::vector<Transaction> transaction_history={}) 
        :name(name), 
        type(type), 
        amount(amount), 
        transaction_history(std::move(transaction_history)) {} 

    Entity(const Entity& entity) 
        :name(entity.name), 
         type(entity.type), 
         amount(entity.amount), 
         transaction_history(entity.transaction_history) {}
    
    Entity& operator=(const Entity& entity) {
        name = entity.name;
        type = entity.type;
        amount = entity.amount;
        transaction_history = entity.transaction_history;
        return *this;
    }

    Type GetType() const {
        return type;
    }

    double GetAmount() const {
        return amount;
    }
    
    std::string GetName() const {
        return name;
    }

    const std::vector<Transaction>& GetHistory() const {
        return transaction_history;
    }

    void AddTransaction(const Transaction& transaction);

    
    absl::flat_hash_set<Type>& LeftsideType() const;

    void Print() const;

    ~Entity() {}
private:
    Type type;
    double amount;
    std::string name;
    std::vector<Transaction> transaction_history;
};

} // namespace yypocketbook


#endif // ENTITY_H_
