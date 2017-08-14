#ifndef CLIENT_H
#define CLIENT_H
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include "connection.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include "stock.h"
#include <chrono>
#include <time.h>

namespace s11n_example {

/// Downloads stock quote information from a server.
class client
{
public:
  /// Constructor starts the asynchronous connect operation.
  client(boost::asio::io_service& io_service,
      const std::string& host, const std::string& service, std::string _request)
    : connection_(io_service)
  {

     set_reqest(_request);

    // Resolve the host name into an IP address.
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(host, service);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
      resolver.resolve(query);

    // Start an asynchronous connect operation.
    boost::asio::async_connect(connection_.socket(), endpoint_iterator,
        boost::bind(&client::handle_connect, this,
          boost::asio::placeholders::error));

  }
  std::string client_ip()
  {
      boost::asio::io_service io_service;
      boost::asio::ip::tcp::socket s(io_service);
      s.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("216.58.193.206"), 80));

      return s.local_endpoint().address().to_string();
  }

  std::time_t get_tick_time()
  {
      std::chrono::time_point<std::chrono::system_clock> start;

      start = std::chrono::system_clock::now();

      return std::chrono::system_clock::to_time_t(start);
  }

  /// Handle completion of a connect operation.
  void handle_connect(const boost::system::error_code& e)
  {
    if (!e)
    {
      // Successfully established connection. Start operation to read the list
      // of stocks. The connection::async_read() function will automatically
      // decode the data that is read from the underlying socket.
      connection_.async_write(s,
          boost::bind(&client::handle_read, this,
            boost::asio::placeholders::error));
      std::cout << "Data has been sent..." << std::endl;
    }
    else
    {
      // An error occurred. Log it and return. Since we are not starting a new
      // operation the io_service will run out of work to do and the client will
      // exit.
      std::cerr << e.message() << std::endl;
    }
  }

  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e)
  {


    // Since we are not starting a new operation the io_service will run out of
    // work to do and the client will exit.
  }
  void set_reqest(std::string &req)
  {
      srand(time(0));
      clock_t start = clock();
      s.dwClientId = client_ip();
      s.cPriority = 1 + rand() % 255;
      s.Data = req;
      clock_t end = clock() - start;
      s.dwTicks = end;
  }

private:
  /// The connection to the server.
  connection connection_;

  /// The data received from the server.

  stock s;
};

} // namespace s11n_example
#endif // CLIENT_H
