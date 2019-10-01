#include "util.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/util/message_differencer.h"

namespace yypocketbook {
namespace {

using ::testing::Test;
using ::google::protobuf::util::MessageDifferencer;
using ::google::protobuf::TextFormat;

class UtilTest : public Test {
protected:
    UtilTest() {
        book_.AddEntity(Entity::ASSET, "Bank Account", 100);
    }

    Book book_;
};

TEST_F(UtilTest, SanityCheck) {
    protobuf::Book protobuf_book;
    ASSERT_TRUE(TextFormat::ParseFromString( 
        R"pb(entities {
                type: ASSET
                name: "Bank Account"
                amount: 100
            })pb", &protobuf_book));
    EXPECT_TRUE(MessageDifferencer::Equivalent(MakeProtoBufBook(book_), protobuf_book));
}

} // namespace
} // namespace yypocketbook

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
