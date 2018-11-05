//
// asio_wrapper.hpp
// ~~~~~~~~~~~~~~~~
//
// Stub to provide installation agnostic way to access ASIO. To use,
// #include "asio_wrapper.hpp"
// and then use the standard asio::* namespace. If you have a boost
// version installed, it will automatically map the namespace.
//
// Note, this requires at least GCC 5.X
//

#if __has_include(<asio.hpp>)
#include <asio.hpp>
#else
#include <boost/asio.hpp>
namespace asio = boost::asio;
#endif
