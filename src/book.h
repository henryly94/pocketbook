#ifndef BOOK_H_
#define BOOK_H_

#include "entity.h"

#include <utility>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"

namespace yypocketbook {

using EntityFeature = std::pair<Entity::Type, std::string>;
using EntityFilter = std::function<bool(const Entity&)>;

class Book {
public:
    explicit Book() {}
    explicit Book(const std::vector<EntityFeature>& entity_features);

    void AddEntity(Entity::Type entity_type, const std::string& entity_name, double amount=0);

    void AddTransaction(double amount, absl::string_view from_account_name, const std::string& to_account_name, absl::string_view detail, absl::Time timestamp);
    
    void LoadTSVConfig(absl::string_view file_path);

    void DumpToDisk(absl::string_view file_path);

    void LoadFromDisk(absl::string_view file_path);

    void Print() const;

    void PrintWithFilter(EntityFilter filter) const;

private:
    absl::flat_hash_map<std::string, Entity> entity_map_; 

};

} // namespace yypocketbook

#endif // BOOK_H_
