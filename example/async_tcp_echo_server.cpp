// Dummy file to test loading of asio

#include "asio_wrapper.hpp"

int main(int argc, char * argv[]){
  asio::io_service io_service;
  io_service.run();
  return 0;
}