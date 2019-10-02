#include "google/protobuf/text_format.h"
#include "google/protobuf/util/message_differencer.h"
#include "util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace yypocketbook {
namespace {

using ::google::protobuf::TextFormat;
using ::google::protobuf::util::MessageDifferencer;
using ::testing::Test;

MATCHER_P(EqualsTransaction, a, "") {
  return arg.type == a.type && arg.amount == a.amount &&
         arg.detail == a.detail && arg.timestamp == a.timestamp;
}

class UtilTest : public Test {
protected:
  UtilTest() {
    book_.AddEntity(Entity::ASSET, "Bank Account", 100);
    transaction_.type = Transaction::CREDIT;
    transaction_.amount = 100;
    transaction_.detail = "Transfer to bank account";
    transaction_.timestamp = absl::FromUnixMicros(1569974247461924);
  }

  Book book_;
  Transaction transaction_;
};

TEST_F(UtilTest, SanityCheck) {
  protobuf::Book protobuf_book;
  ASSERT_TRUE(TextFormat::ParseFromString(
      R"pb(entities {type: ASSET name: "Bank Account" amount: 100})pb",
      &protobuf_book));
  EXPECT_TRUE(MessageDifferencer::Equivalent(Util::MakeProtobufBook(book_),
                                             protobuf_book));
}

TEST_F(UtilTest, Transaction) {
  EXPECT_THAT(transaction_, EqualsTransaction(Util::MakeTransaction(
                                Util::MakeProtobufTransaction(transaction_))));
}

} // namespace
} // namespace yypocketbook

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
