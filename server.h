#ifndef SERVER_H
#define SERVER_H
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include "connection.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include "stock.h"
#include <queue>
#include <thread>
#include <mutex>
#include <time.h>
#include <condition_variable>
#include <fstream>
#include <sstream>
#include <map>


namespace s11n_example {

/// Serves stock quote information to any client that connects to it.
class server
{
public:
  /// Constructor opens the acceptor and starts waiting for the first incoming
  /// connection.
  server(boost::asio::io_service& io_service, unsigned short port)
    : acceptor_(io_service,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
  {

      std::cout << "Server is runing..." << std::endl;


    // Start an accept operation for a new connection.
    connection_ptr new_conn(new connection(acceptor_.get_io_service()));
    acceptor_.async_accept(new_conn->socket(),
        boost::bind(&server::handle_accept, this,
          boost::asio::placeholders::error, new_conn));
  }

  /// Handle completion of a accept operation.
  void handle_accept(const boost::system::error_code& e, connection_ptr conn)
  {
    if (!e)
    {
      // Successfully accepted a new connection. Send the list of stocks to the
      // client. The connection::async_write() function will automatically
      // serialize the data structure for us.
      conn->async_read(s,
          boost::bind(&server::handle_write, this,
            boost::asio::placeholders::error, conn));
    }

    // Start an accept operation for a new connection.
    connection_ptr new_conn(new connection(acceptor_.get_io_service()));
    acceptor_.async_accept(new_conn->socket(),
        boost::bind(&server::handle_accept, this,
          boost::asio::placeholders::error, new_conn));
  }


  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e, connection_ptr conn)
  {
    // Nothing to do. The socket will be closed automatically when the last
    // reference to the connection object goes away.
      std::time_t t = std::time(nullptr);

      std::cout << std::put_time(std::localtime(&t), "%c") << " get log from: " << s.dwClientId << " info <" << s.Data << ">" << std::endl;

      std::thread producer{[&]()
                           {
                            std::lock_guard<std::mutex>lock(mute);
                            stocks_.push(s);
                            cond.notify_all();
                           }};

      std::thread write_to_file{[&]()
                           {

                             std::unique_lock<std::mutex> lock(mute);
                             while (stocks_.empty()) {
                                 cond.wait(lock);
                             }
                             std::ofstream out("server.log", std::ios_base::out | std::ios_base::app );



                             while (!stocks_.empty()) {
                                 std::ostringstream os;
                                 os << "log time: " << clock() << std::endl << "id " << stocks_.top().dwClientId \
                                    << std::endl << "priority " << static_cast<int>(stocks_.top().cPriority) << \
                                       std::endl << "time request " << stocks_.top().dwTicks << std::endl;
                                 out << os.str();
                                 stocks_.pop();
                             }
                             out.close();
                           }};

        producer.join();
        write_to_file.join();

  }

private:
  /// The acceptor object used to accept incoming socket connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The data to be sent to each client.
  std::priority_queue<stock> stocks_;
  stock s;
  std::mutex mute;
  std::condition_variable cond;
};

} // namespace s11n_example
#endif // SERVER_H
