#ifndef BUTTER_PLUGOUT_TEST_SUITE_HPP
#define BUTTER_PLUGOUT_TEST_SUITE_HPP


#include <memory>

namespace butter { class base_generator; } 
namespace butter { class basic_style; } 
namespace butter { class bjam_generator; } 
namespace butter { class butter_constants; } 
namespace butter { class cmake_generator; } 
namespace butter { class compound_artifact; } 
namespace butter { class const_token_iterator; } 
namespace butter { class gmake_generator; } 
namespace butter { class jam_generator; } 
namespace butter { class location; } 
namespace butter { class log; } 
namespace butter { class pathcmp; } 
class UmlCom;

/**
 * Test core functionality that is dependent on bouml.
 */
class plugout_test_suite
{
  public:
    /**
     * Class to manage connection between bouml and test run.
     */
    class umlcom_connection
    {
      private:
        umlcom_connection(): connected_( false ) {}


      public:
        ~umlcom_connection();


      private:
        umlcom_connection(umlcom_connection && source) = delete;

        umlcom_connection(const umlcom_connection & source) = delete;

        umlcom_connection & operator=(const umlcom_connection & source) = delete;

        /**
         *  If connection has been made.
         */
        bool connected_;


      public:
        /**
         *  Ensure connection to bouml a returns a reference to the connection object.
         */
        static umlcom_connection const& connection(char** argv);


      private:
        /**
         *  RAII object for connection to bouml
         */
        static std::unique_ptr< umlcom_connection > singleton_;


      public:
        bool connected() const
        {
          return this->connected_;
        }


    };
    
    





















};

#endif
