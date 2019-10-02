#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "absl/time/time.h"

#include "book.h"

using yypocketbook::Book;
using yypocketbook::Entity;

int main() {
  std::cout << "Hello Book!\n";
  Book book;
  book.LoadFromDisk("../src/testdata/checkpoint.tsv");
  book.AddTransaction(100, "Amex Gold", "3C product", "By a monitor",
                      absl::Now());
  book.AddTransaction(100, "Cash", "3C product", "By another monitor",
                      absl::Now());
  book.AddTransaction(2500, "Salary", "Chase Saving", "Salary for Sept",
                      absl::Now());
  book.PrintWithFilter([&](const Entity &entity) {
    return entity.GetType() == Entity::EXPENSE ||
           entity.GetType() == Entity::LIABILITY;
  });
  book.Print();
  book.DumpToDisk("../src/testdata/checkpoint.tsv");
  return 0;
}
