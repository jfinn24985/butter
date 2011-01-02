#ifndef BUTTER_CONFIG_H
#define BUTTER_CONFIG_H 1
#include <stdexcept>

#define BUTTER_ALWAYS(X,Y) do { if (! (X)) { throw std::runtime_error(std::string(Y)); } } while (false)

#define BUTTER_REQUIRE BUTTER_ALWAYS
#ifdef NO_DBC
#define BUTTER_CHECK(X,Y)
#define BUTTER_ENSURE(X,Y)
#else
#define BUTTER_CHECK BUTTER_ALWAYS
#define BUTTER_ENSURE BUTTER_ALWAYS
#endif

// To handle substandard std::auto_ptr without
// a reset method
#ifdef _MSC_VER
#define MSC_NO_RESET(X,Y) X = Y
#else
#define MSC_NO_RESET(X,Y) X.reset
#endif

#endif
