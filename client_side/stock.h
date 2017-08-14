#ifndef STOCK_H
#define STOCK_H



#include <string>

typedef unsigned char BYTE;
typedef unsigned long DWORD;


namespace s11n_example {

/// Structure to hold information about a single stock.
struct stock
{
  BYTE cPriority; //приоритет запроса 0 – 255 (0 – наивысший приоритет)
  DWORD dwTicks; //время формирования запроса в системных тиках
  std::string dwClientId; //уникальный идентификатор клиента
  std::string Data;
  template <typename Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & cPriority;
    ar & dwTicks;
    ar & dwClientId;
    ar & Data;
  }
};

} // namespace s11n_example
#endif // STOCK_H
