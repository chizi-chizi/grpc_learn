/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <grpcpp/resource_quota.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello ");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
  Status SayHelloAgain1(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello again1");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
  Status SayHelloAgain2(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello again2");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
  Status SayHelloAgain3(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello again3");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
  Status SayHelloAgain4(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello again4");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
  Status SayHelloAgain5(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello again5");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
  Status SayHelloAgain6(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello again6");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
  Status SayHelloAgain7(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello again7");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
  Status SayHelloAgain8(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello again8");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
  Status SayHelloAgain9(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello again9");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;
  grpc::ResourceQuota resource_quota; 
  resource_quota.SetMaxThreads(10);

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.SetSyncServerOption(builder.MIN_POLLERS, 1);
  builder.SetSyncServerOption(builder.MAX_POLLERS, 2);
  builder.SetResourceQuota(resource_quota);
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

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
