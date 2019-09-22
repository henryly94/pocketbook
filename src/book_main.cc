#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"

#include "book.h"

using yypocketbook::Entity;
using yypocketbook::Book;

int main() {
    std::cout << "Hello Book!\n";
    Book book;
    book.LoadTSV("../src/testdata/book_config.tsv");
    book.AddTransaction(100, "Amex Gold", "3C product", "By a monitor", absl::Now());
    book.AddTransaction(100, "Cash", "3C product", "By another monitor", absl::Now());
    book.AddTransaction(2500, "Salary", "Chase Saving", "Salary for Sept", absl::Now());
    /*
    book.PrintWithFilter(
        [&](const Entity& entity) {
            return entity.GetType() == Entity::EXPENSE || entity.GetType() == Entity::LIABILITY;
        });
        */
    book.Print();
    return 0;
}
