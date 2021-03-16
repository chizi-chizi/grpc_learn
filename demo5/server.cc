
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "test.h"

#ifdef BAZEL_BUILD
#include "./test.grpc.pb.h"
#else
#include "test.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using test::Test;
using test::Request;
using test::Person;
using namespace std;


// Logic and data behind the server's behavior.
class TestServiceImpl final : public Test::Service {
  Status GetPerson(ServerContext* context,
                   ServerReaderWriter<Person, Request>* stream) override {
    Request request;
    int i;
    while (stream->Read(&request)) {
      Person person;
      
      string pbMsg;
      char* buf= NULL;   // 初始化为 null 的指针
      int len;
      buf = new char[128];
      
      getPbMsg(buf, &len);
      std::cout<<"len"<<len<<std::endl;
      pbMsg.assign(buf, 0, len);
        
      person.ParseFromString(pbMsg);
      stream->Write(person);
   }
   return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  TestServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case, it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
