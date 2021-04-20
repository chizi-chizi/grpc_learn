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
struct cmd
{
    int min_poll;
    int max_poll;
    int thread_max_num;
};

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello ");
    sleep(5);
    reply->set_message(prefix + request->name());
    return Status::OK;
  }
};

void RunServer(struct cmd& cmd_opt) {
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;
  grpc::ResourceQuota resource_quota; 
  resource_quota.SetMaxThreads(cmd_opt.thread_max_num);

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.SetSyncServerOption(builder.MIN_POLLERS, cmd_opt.min_poll);
  builder.SetSyncServerOption(builder.MAX_POLLERS, cmd_opt.max_poll);
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
int parse_cmd(int argc, char** argv, struct cmd& cmd_opt)
{
    int opt = 0;
    int min_poll = 0;
    int max_poll = 0;
    int thread_max_num = 0;
    while((opt = getopt(argc, argv, "hi:a:t:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                printf("Usage:%s -i <min_poll_num> -a <max_poll_num> -t <thread_pool_max_num>\n", argv[0]);
                return -1;
            case 'i':
                min_poll = atoi(optarg);
                break;
            case 'a':
                max_poll = atoi(optarg);
                break;
            case 't':
                thread_max_num = atoi(optarg);
                break;
            default:
                printf("Usage:%s -i <min_poll_num> -a <max_poll_num> -t <thread_pool_max_num>\n", argv[0]);
                return -1;
         }
    }
    if (min_poll != 0 && max_poll !=0  && max_poll > min_poll && thread_max_num != 0)
    {
       cmd_opt.min_poll = min_poll; 
       cmd_opt.max_poll = max_poll; 
       cmd_opt.thread_max_num =  thread_max_num; 
        return 0;
    }
    else
    {
        printf("Usage:%s -i <min_poll_num> -a <max_poll_num> -t <thread_pool_max_num>\n", argv[0]);
        return 1;
    }

}
int main(int argc, char** argv) {
  
  struct cmd cmd_opt;
  int iRet = 0;
  iRet = parse_cmd(argc, argv, cmd_opt);
  if(iRet == 0 )
  {
    RunServer(cmd_opt);
  }
  return 0;
}
