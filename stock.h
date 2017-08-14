#ifndef STOCK_H
#define STOCK_H


#include <string>

typedef unsigned char BYTE;
typedef unsigned long DWORD;

namespace s11n_example {

/// Structure to hold information about a single stock.
struct stock
{
    BYTE cPriority;
    DWORD dwTicks;
    std::string dwClientId;
    std::string Data;
    friend bool operator <(const stock& lhe, const stock& rhe)
    {
        return lhe.cPriority > rhe.cPriority;
    }
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
