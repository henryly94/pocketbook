#include <grpcpp/grpcpp.h>

#include "service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using yypocketbook::protobuf::GetBookRequest;
using yypocketbook::protobuf::GetBookReply;
using yypocketbook::protobuf::BookKeeping;


class BookKeepingClient {
 public:
  BookKeepingClient(std::shared_ptr<Channel> channel)
      : stub_(BookKeeping::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string GetBook() {
    // Data we are sending to the server.
    GetBookRequest request;
    request.set_time_usec(0);

    // Container for the data we expect from the server.
    GetBookReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetBook(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.book().DebugString();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<BookKeeping::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  BookKeepingClient book_keeping_client(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = book_keeping_client.GetBook();
  std::cout << "BookKeeping received: " << reply << std::endl;

  return 0;
}
