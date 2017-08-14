#include "client.h"


int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc != 4)
    {
      std::cerr << "Usage: client <host> <port> <data>" << std::endl;
      return 1;
    }

    boost::asio::io_service io_service;
    s11n_example::client client(io_service, argv[1], argv[2], argv[3]);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
