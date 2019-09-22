#include <iostream>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"

namespace yypocketbook {

struct Entity {
    enum EntityType {
        ASSET,
        LIABILITY,
        EQUITY
    };

};

class Book {
public:
    

};

}

using yypocketbook::Entity;

int main() {
    std::cout << "Hello Book!\n" << Entity::ASSET;
    absl::flat_hash_map<absl::string_view, yypocketbook::Entity> maps;
    /*
    Book book({Entity::ASSET, "Asset"}, {Entity::LIABILITY, "Liability"}, {Entity::EQUITY, "Equity"});
    book.add_value(100, "Asset", "Equity", "By a monitor");
    book.print();
    */
    return 0;
}