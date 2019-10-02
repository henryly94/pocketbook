#include <memory>

#include "book.h"
#include "messages.pb.h"
#include "service.grpc.pb.h"
#include "util.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/codegen/status.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using yypocketbook::protobuf::BookKeeping;
using yypocketbook::protobuf::GetBookReply;
using yypocketbook::protobuf::GetBookRequest;

class BookKeepingServiceImpl final : public BookKeeping::Service {
  Status GetBook(ServerContext *context, const GetBookRequest *request,
                 GetBookReply *reply) override {
    yypocketbook::Book book;
    book.LoadFromDisk("../src/testdata/checkpoint.tsv");
    *reply->mutable_book() =
        std::move(yypocketbook::Util::MakeProtobufBook(book));
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  BookKeepingServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main() {
  RunServer();
  return 0;
}
