#include <gflags/gflags.h>
#include <grpc++/grpc++.h>
#include <spdlog/spdlog.h>
#include <iostream>

#include "faiss.grpc.pb.h"
#include "service_impl.h"

DEFINE_string(host, "0.0.0.0", "host of faiss server");
DEFINE_string(port, "8080", "port of faiss server");
DEFINE_string(file_path, "", "index file name");

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::string address = FLAGS_host + ":" + FLAGS_port;
  auto logger = spdlog::stdout_color_mt("console");
  FaissServiceImpl service(logger, FLAGS_file_path.c_str());
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  logger->info("Server run on {0}", address);
  server->Wait();
  return 0;
}
