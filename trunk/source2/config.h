#ifndef BUTTER_CONFIG_H
#define BUTTER_CONFIG_H 1
#include <string>
#include <stdexcept>

#ifdef NO_DBC
#define NO_DBC 1
#else
#define NO_DBC 0
#endif

inline void BUTTER_ALWAYS(bool X, std::string Y)
{
   if (! X) throw std::runtime_error(Y);
}

inline void BUTTER_REQUIRE(bool X, std::string Y)
{
   BUTTER_ALWAYS(X,Y);
}

inline void BUTTER_CHECK(bool X, std::string Y)
{
   if (! NO_DBC) BUTTER_ALWAYS(X,Y);
}

inline void BUTTER_ENSURE(bool X, std::string Y)
{
   if (! NO_DBC) BUTTER_ALWAYS(X,Y);
}

#undef NO_DBC

#endif
