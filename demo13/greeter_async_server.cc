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
#include <memory>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>
#include <grpc/impl/codegen/gpr_types.h>
#include <grpcpp/impl/codegen/time.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif
using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;
using grpc::CompletionQueue;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;
int g_exit = 0;
void signalHandler(int signo)
{
        printf("receive:%d\n", signo);
        g_exit = 1;
        return;
}
class ServerImpl final {
 public:
  ~ServerImpl() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
  }
  // There is no shutdown handling in this code.
  void Run() {
    std::string server_address("0.0.0.0:50051");
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&service_);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    cq_ = builder.AddCompletionQueue();
    // Finally assemble the server.
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;
    // Proceed to the server's main loop.
    HandleRpcs();
  }
 private:
  // Class encompasing the state and logic needed to serve a request.
  class CallData {
   public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    CallData(Greeter::AsyncService* service, ServerCompletionQueue* cq)
        : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
      // Invoke the serving logic right away.
        std::cout << "-----begin----CallData" << std::endl;
      Proceed();
    }
        ~CallData(){
                std::cout << "----end-----~~~~CallData" << std::endl;
        }
    void Proceed() {
      if (status_ == CREATE) {
        // Make this instance progress to the PROCESS state.
        status_ = PROCESS;
        std::cout << "----status:Create" << std::endl;
        // As part of the initial CREATE state, we *request* that the system
        // start processing SayHello requests. In this request, "this" acts are
        // the tag uniquely identifying the request (so that different CallData
        // instances can serve different requests concurrently), in this case
        // the memory address of this CallData instance.
        service_->RequestSayHello(&ctx_, &request_, &responder_, cq_, cq_,
                                  this);
      } else if (status_ == PROCESS) {
        std::cout << "----status:Process" << std::endl;
        // Spawn a new CallData instance to serve new clients while we process
        // the one for this CallData. The instance will deallocate itself as
        // part of its FINISH state.
        CallData*  pcallData2 = new CallData(service_, cq_);
        std::cout << "pcallData2 address: " << pcallData2 << std::endl;
        std::cout << "pcallData2 service address: " << service_<< std::endl;
        std::cout << "pcallData2 cq address: " << cq_<< std::endl;
        //new CallData(service_, cq_);
        // The actual processing.
        std::string prefix("Hello ");
        reply_.set_message(prefix + request_.name());
        sleep(2);
        std::cout << "sleep 4s" << std::endl;
        // And we are done! Let the gRPC runtime know we've finished, using the
        // memory address of this instance as the uniquely identifying tag for
        // the event.
        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this);
      } else {
        std::cout << "----status:Finish" << std::endl;
        GPR_ASSERT(status_ == FINISH);
        // Once in the FINISH state, deallocate ourselves (CallData).
        delete this;
      }
    }
   private:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    Greeter::AsyncService* service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* cq_;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;
    // What we get from the client.
    HelloRequest request_;
    // What we send back to the client.
    HelloReply reply_;
    // The means to get back to the client.
    ServerAsyncResponseWriter<HelloReply> responder_;
    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.
  };
  // This can be run in multiple threads if needed.
  void HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    /* comments by chizi
    unique_ptr类具有get方法， get方法的作用是 returns a pointer to the managed object
    可以参考https://en.cppreference.com/w/cpp/memory/unique_ptr
    此处cq_.get()就是返回ServerCompletionQueue对象。
    */
    CallData*  pcallData = new CallData(&service_, cq_.get());
    std::cout << "pcallData address: " << pcallData << std::endl;
    std::cout << "pcallData service address: " << &service_<< std::endl;
    std::cout << "pcallData cq address: " << &cq_<< std::endl;
    std::cout << "pcallData cq address: " << cq_.get()<< std::endl;
    void* tag;  // uniquely identifies a request.
    bool ok;
    CompletionQueue::NextStatus nextStatus;
    gpr_timespec time;
    time.tv_sec = 2;
    time.tv_nsec = 0;
    time.clock_type = GPR_TIMESPAN;
    while (!g_exit) {
        std::cout<<"...............while.........."<<std::endl;
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
      //GPR_ASSERT(cq_->Next(&tag, &ok));
      nextStatus = cq_->AsyncNext(&tag,&ok, time);
        std::cout << "status:" << nextStatus<< std::endl;
        std::cout << "ok:" << ok<< std::endl;
        if (nextStatus == CompletionQueue::NextStatus::TIMEOUT){
                std::cout << "timeout continue.." << std::endl;
                continue;
        }
        else if (nextStatus == CompletionQueue::NextStatus::SHUTDOWN){
                std::cout << "shutdown!!" << std::endl;
        }
        else
        {
                std::cout << "Got_EVENT" << std::endl;
                //GPR_ASSERT(ok);
                static_cast<CallData*>(tag)->Proceed();
        }
        std::cout<<"$$$$$$$$$$$$$$$$$$$$$$$$@@@@@@"<<std::endl;
    }
    std::cout<<"*****************************$$$$"<<std::endl;
    nextStatus = cq_->AsyncNext(&tag,&ok, time);
    std::cout << "status:" << nextStatus<< std::endl;
    std::cout << "ok:" << ok<< std::endl;
  }
  std::unique_ptr<ServerCompletionQueue> cq_;
  Greeter::AsyncService service_;
  std::unique_ptr<Server> server_;
};
int main(int argc, char** argv) {
  signal(SIGINT, signalHandler);
  ServerImpl server;
  server.Run();
  return 0;
}
