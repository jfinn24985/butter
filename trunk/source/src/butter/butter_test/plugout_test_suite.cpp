
#define BOOST_TEST_MODULE core_test
#include <boost/test/unit_test.hpp>

#include "../src/butter/butter_test/plugout_test_suite.h"
#include "butter/base_generator.h"
#include "butter/basic_style.h"
#include "butter/bjam_generator.h"
#include "butter/butter_constants.h"
#include "butter/cmake_generator.h"
#include "butter/compound_artifact.h"
#include "butter/const_token_iterator.h"
#include "butter/gmake_generator.h"
#include "butter/jam_generator.h"
#include "butter/location.h"
#include "butter/utility.h"
#include "bouml/UmlCom.h"

// Manuals
#include "version.h"
// -
plugout_test_suite::umlcom_connection::~umlcom_connection() 
{
if( this->connected_ )
{
   UmlCom::bye();
   UmlCom::close();
}

}

plugout_test_suite::umlcom_connection const& plugout_test_suite::umlcom_connection::connection(char** argv)

{
if( nullptr == umlcom_connection::singleton_ )
{
  umlcom_connection::singleton_.reset( new umlcom_connection );
  umlcom_connection::singleton_->connected_ = ( UmlCom::connect( QString( boost::unit_test::framework::master_test_suite().argv[1] ).toUInt() ) ? true : false );
}
return *umlcom_connection::singleton_.get();

}

/**
 *  RAII object for connection to bouml
 */
std::unique_ptr< plugout_test_suite::umlcom_connection > plugout_test_suite::umlcom_connection::singleton_;

/**
 * Test lifetime methods and accessors of values in ctor
 * list.
 */
BOOST_AUTO_TEST_CASE( umlcom_connection_test )
{
if( plugout_test_suite::umlcom_connection::connection( boost::unit_test::framework::master_test_suite().argv ).connected() )
{
  try
  {
    UmlCom::trace( "<b>butter unit test:</b> " + butter::butter_constants::BUTTER_VERSION + "<br>" );
    BOOST_REQUIRE_MESSAGE( nullptr != UmlPackage::getProject (), "Error: no project defined!<br>" );
    // Parse the project into a location tree
    std::unique_ptr< butter::location > base_( butter::location::parse_project( *UmlPackage::getProject() ) );
  }
  catch ( const std::exception & a_err )
  {
    UmlCom::trace( a_err.what () );
  }
  catch ( const char * a_what )
  {
    UmlCom::trace( a_what );
  }
  catch ( ... )
  {
    UmlCom::trace( "Caught unknown exception." );
  }

  UmlCom::trace( "<b>butter unit test complete<br>" );
  // must be called to cleanly inform that all is done
}

}

/**
 * Test lifetime methods and accessors of values in ctor
 * list.
 */
BOOST_AUTO_TEST_CASE( umlcom_connection_test2 )
{
if( plugout_test_suite::umlcom_connection::connection( boost::unit_test::framework::master_test_suite().argv ).connected() )
{
  try
  {
    UmlCom::trace( "<b>butter unit test:</b> " + butter::butter_constants::BUTTER_VERSION + "<br>" );
    BOOST_REQUIRE_MESSAGE( nullptr != UmlPackage::getProject (), "Error: no project defined!<br>" );
    // Parse the project into a location tree
    std::unique_ptr< butter::location > base_( butter::location::parse_project( *UmlPackage::getProject() ) );
  }
  catch ( const std::exception & a_err )
  {
    UmlCom::trace( a_err.what () );
  }
  catch ( const char * a_what )
  {
    UmlCom::trace( a_what );
  }
  catch ( ... )
  {
    UmlCom::trace( "Caught unknown exception." );
  }

  UmlCom::trace( "<b>butter unit test complete<br>" );
  // must be called to cleanly inform that all is done
}

}

/**
 * Test lifetime methods and accessors of values in ctor
 * list.
 */
BOOST_AUTO_TEST_CASE( location_lifetime )
{
//XX// Lifetime method tests
//XXBOOST_CHECK( not std::is_default_constructible< butter::location >::type{} );
//XXBOOST_CHECK( not std::is_copy_constructible< butter::location >::type{} );
//XXBOOST_CHECK( not std::is_move_constructible< butter::location >::type{} );
//XXBOOST_CHECK( not (std::is_assignable< butter::location, butter::location >::type{}) );
//XXBOOST_CHECK( not std::has_virtual_destructor< butter::location >::type{});
//XX
//XXbutter::pathcmp p1( "./src/test" );
//XXbutter::location l1( p1 );
//XX
//XXBOOST_CHECK_EQUAL( l1.path().path(), p1.path() );
//XXBOOST_CHECK( l1.parent() == nullptr );
//XXBOOST_CHECK( l1.packages().isEmpty() );
//XXBOOST_CHECK_EQUAL( l1.packages().count(), 0 );
//XXBOOST_CHECK_EQUAL( l1.packages().size(), 0 );
//XXBOOST_CHECK( l1.children().isEmpty() );
//XXBOOST_CHECK_EQUAL( l1.children().count(), 0 );
//XXBOOST_CHECK_EQUAL( l1.children().size(), 0 );
//XX
//XXBOOST_CHECK_EQUAL( l1.full_path().path(), p1.path() );
//XX

}

/**
 * Test location methods and accessors
 * 
 * Tested:
 *  children, create_as_child, full_path, location, packages, parent,
 *  path, serialize
 * Untested:
 *   add_package, compare_file_to_string, create_common_parent, 
 *   create_uml_document, find, find_uml_document, parse_project,
 *   write_documents, write_uml_documents.
 */
BOOST_AUTO_TEST_CASE( location_methods )
{
//XXbutter::pathcmp base_path( "./src/test" );
//XXbutter::location root( base_path );
//XX
//XXBOOST_CHECK_EQUAL( root.path().path(), base_path.path() );
//XXBOOST_CHECK( root.parent() == nullptr );
//XXBOOST_CHECK( root.packages().isEmpty() );
//XXBOOST_CHECK_EQUAL( root.packages().size(), 0 );
//XXBOOST_CHECK_EQUAL( root.packages().count(), 0 );
//XXBOOST_CHECK( root.children().isEmpty() );
//XX
//XXBOOST_CHECK_EQUAL( root.full_path().path(), base_path.path() );
//XX
//XXbutter::pathcmp leaf_path1( "src/test/subdir1" );
//XXbutter::location * child1 = root.create_as_child( leaf_path1 );
//XX
//XXBOOST_CHECK_EQUAL( child1->path().path(), "subdir1" );
//XXBOOST_CHECK_EQUAL( child1->full_path().path(), "src/test/subdir1" );
//XXBOOST_CHECK( child1->parent() != nullptr );
//XXBOOST_CHECK( child1->packages().isEmpty() );
//XXBOOST_CHECK_EQUAL( child1->packages().size(), 0 );
//XXBOOST_CHECK_EQUAL( child1->packages().count(), 0 );
//XXBOOST_CHECK( child1->children().isEmpty() );
//XXBOOST_CHECK_EQUAL( child1->children().size(), 0 );
//XXBOOST_CHECK_EQUAL( child1->children().count(), 0 );
//XXBOOST_CHECK((not root.children().isEmpty()) );
//XXBOOST_CHECK_EQUAL( root.children().size(), 1 );
//XXBOOST_CHECK_EQUAL( root.children().count(), 1 );
//XXBOOST_CHECK( &root == child1->parent() );
//XX
//XXbutter::pathcmp leaf_path2( "./src/test/subdir2" );
//XXbutter::location * child2 = root.create_as_child( leaf_path2 );
//XX
//XXBOOST_CHECK_EQUAL( child2->path().path(), "subdir2" );
//XXBOOST_CHECK_EQUAL( child2->full_path().path(), "src/test/subdir2" );
//XXBOOST_CHECK( child2->parent() != nullptr );
//XXBOOST_CHECK( child2->packages().isEmpty() );
//XXBOOST_CHECK_EQUAL( child2->packages().size(), 0 );
//XXBOOST_CHECK_EQUAL( child2->packages().count(), 0 );
//XXBOOST_CHECK( child2->children().isEmpty() );
//XXBOOST_CHECK_EQUAL( child2->children().size(), 0 );
//XXBOOST_CHECK_EQUAL( child2->children().count(), 0 );
//XXBOOST_CHECK( (not root.children().isEmpty()) );
//XXBOOST_CHECK_EQUAL( root.children().size(), 2 );
//XXBOOST_CHECK_EQUAL( root.children().count(), 2 );
//XXBOOST_CHECK( &root == child2->parent() );
//XX
//XXif (DEBUG)
//XX{
//XX  QString result;
//XX  QTextOStream qos( &result );
//XX  root.serialize( qos );
//XX  QString canon( "<<BEGIN location [src/test]\nChildren:\n[0] = subdir1\n[1] = subdir2\n<<END location [src/test]\n<<BEGIN location [src/test/subdir1]\n<<END location [src/test/subdir1]\n<<BEGIN location [src/test/subdir2]\n<<END location [src/test/subdir2]\n" );
//XX  BOOST_CHECK_EQUAL( result, canon );
//XX
//XX}

}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( bjam_generator_lifetime )
{
//XX// Lifetime method tests
//XXBOOST_CHECK( not std::is_default_constructible< butter::bjam_generator >::type{} );
//XXBOOST_CHECK( not std::is_copy_constructible< butter::bjam_generator >::type{} );
//XXBOOST_CHECK( not std::is_move_constructible< butter::bjam_generator >::type{} );
//XXBOOST_CHECK( not (std::is_assignable< butter::bjam_generator, butter::bjam_generator >::type{}) );
//XXBOOST_CHECK( std::has_virtual_destructor< butter::bjam_generator >::type{});
//XX
//XX// General variables
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::build_file_name, "Jamfile" );
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::build_file_sysname, "Jamroot" );
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::rules_name, "local.jam" );
//XX// BOOST_CHECK_EQUAL( butter::bjam_generator::default_rules, "Jamfile" );
//XX
//XX// style
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::style.comment_string(), "#" );
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::style.end_comment_string(), "" );
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::style.end_phrase(), "##END:" );
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::style.end_end_phrase(), "" );
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::style.start_phrase(), "##START:" );
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::style.end_start_phrase(), "" );
//XXBOOST_CHECK_EQUAL( butter::bjam_generator::style.name(), "boost" );
//XX
//XXauto pgen = butter::bjam_generator::create();
//XXBOOST_CHECK( pgen );
//XXBOOST_CHECK( nullptr != pgen.get() );
//XX
//XXBOOST_CHECK_EQUAL( pgen->target_NAME(), "" );
//XXBOOST_CHECK_EQUAL( pgen->root_dir().path(), "." );

}

/**
 * Test lifetime methods and static attributes
 */
BOOST_AUTO_TEST_CASE( cmake_generator_lifetime )
{
//XX// Lifetime method tests
//XXBOOST_CHECK( not std::is_default_constructible< butter::cmake_generator >::type{} );
//XXBOOST_CHECK( not std::is_copy_constructible< butter::cmake_generator >::type{} );
//XXBOOST_CHECK( not std::is_move_constructible< butter::cmake_generator >::type{} );
//XXBOOST_CHECK( not (std::is_assignable< butter::cmake_generator, butter::cmake_generator >::type{}) );
//XXBOOST_CHECK( std::has_virtual_destructor< butter::cmake_generator >::type{});
//XX
//XX// Common values.
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::build_file_name, "CMakeLists.txt" );
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::build_file_sysname, "CMakeLists.txt" );
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::rules_name, "local.cmake" );
//XX// BOOST_CHECK_EQUAL( butter::cmake_generator::default_rules, "Jamfile" );
//XX
//XX// CMAKE specific
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::cmake_minimum_required_, "cmake_minimum_required(VERSION 2.6)" );
//XX
//XX// Jam style
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::style.comment_string(), "#" );
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::style.end_comment_string(), "" );
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::style.end_phrase(), "##END:" );
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::style.end_end_phrase(), "" );
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::style.start_phrase(), "##START:" );
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::style.end_start_phrase(), "" );
//XXBOOST_CHECK_EQUAL( butter::cmake_generator::style.name(), "cmake" );
//XX
//XXauto pgen = butter::bjam_generator::create();
//XXBOOST_CHECK( pgen );
//XXBOOST_CHECK( nullptr != pgen.get() );
//XX
//XXBOOST_CHECK_EQUAL( pgen->target_NAME(), "" );
//XXBOOST_CHECK_EQUAL( pgen->root_dir().path(), "." );

}

/**
 * Test lifetime methods and static attributes
 */
BOOST_AUTO_TEST_CASE( gmake_generator_lifetime )
{
//XX// Lifetime method tests
//XXBOOST_CHECK( not std::is_default_constructible< butter::gmake_generator >::type{} );
//XXBOOST_CHECK( not std::is_copy_constructible< butter::gmake_generator >::type{} );
//XXBOOST_CHECK( not std::is_move_constructible< butter::gmake_generator >::type{} );
//XXBOOST_CHECK( not (std::is_assignable< butter::gmake_generator, butter::gmake_generator >::type{}) );
//XXBOOST_CHECK( std::has_virtual_destructor< butter::gmake_generator >::type{});
//XX
//XX// Common values.
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::build_file_name, "makefile" );
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::build_file_sysname, "makefile" );
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::rules_name, "M_sys.mk M_cl.mk M_gcc.mk M_unix.mk M_Windows_NT.mk" );
//XX// BOOST_CHECK_EQUAL( butter::gmake_generator::default_rules, "Jamfile" );
//XX
//XX// Gnu make specific
//XX// BOOST_CHECK_EQUAL( butter::gmake_generator::default_rules_sys, "" );
//XX
//XX// style
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::style.comment_string(), "#" );
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::style.end_comment_string(), "" );
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::style.end_phrase(), "##END:" );
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::style.end_end_phrase(), "" );
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::style.start_phrase(), "##START:" );
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::style.end_start_phrase(), "" );
//XXBOOST_CHECK_EQUAL( butter::gmake_generator::style.name(), "make" );
//XX
//XXauto pgen = butter::bjam_generator::create();
//XXBOOST_CHECK( pgen );
//XXBOOST_CHECK( nullptr != pgen.get() );
//XX
//XXBOOST_CHECK_EQUAL( pgen->target_NAME(), "" );
//XXBOOST_CHECK_EQUAL( pgen->root_dir().path(), "." );

}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( jam_generator_lifetime )
{
//XX// Lifetime method tests
//XXBOOST_CHECK( not std::is_default_constructible< butter::jam_generator >::type{} );
//XXBOOST_CHECK( not std::is_copy_constructible< butter::jam_generator >::type{} );
//XXBOOST_CHECK( not std::is_move_constructible< butter::jam_generator >::type{} );
//XXBOOST_CHECK( not (std::is_assignable< butter::jam_generator, butter::jam_generator >::type{}) );
//XXBOOST_CHECK( std::has_virtual_destructor< butter::jam_generator >::type{});
//XX
//XX// General variables.
//XXBOOST_CHECK_EQUAL( butter::jam_generator::build_file_name, "Jamfile" );
//XXBOOST_CHECK_EQUAL( butter::jam_generator::build_file_sysname, "Jamfile" );
//XXBOOST_CHECK_EQUAL( butter::jam_generator::rules_name, "Jamrules" );
//XX// BOOST_CHECK_EQUAL( butter::jam_generator::default_rules, "Jamfile" );
//XX
//XX// Jam style
//XXBOOST_CHECK_EQUAL( butter::jam_generator::style.comment_string(), "#" );
//XXBOOST_CHECK_EQUAL( butter::jam_generator::style.end_comment_string(), "" );
//XXBOOST_CHECK_EQUAL( butter::jam_generator::style.end_phrase(), "##END:" );
//XXBOOST_CHECK_EQUAL( butter::jam_generator::style.end_end_phrase(), "" );
//XXBOOST_CHECK_EQUAL( butter::jam_generator::style.start_phrase(), "##START:" );
//XXBOOST_CHECK_EQUAL( butter::jam_generator::style.end_start_phrase(), "" );
//XXBOOST_CHECK_EQUAL( butter::jam_generator::style.name(), "standard" );
//XX
//XXauto pgen = butter::bjam_generator::create();
//XXBOOST_CHECK( pgen );
//XXBOOST_CHECK( nullptr != pgen.get() );
//XX
//XXBOOST_CHECK_EQUAL( pgen->target_NAME(), "" );
//XXBOOST_CHECK_EQUAL( pgen->root_dir().path(), "." );

}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( compound_artifact_lifetime )
{
//XX// Lifetime method tests
//XXBOOST_CHECK( not std::is_default_constructible< butter::compound_artifact >::type{} );
//XXBOOST_CHECK( not std::is_copy_constructible< butter::compound_artifact >::type{} );
//XXBOOST_CHECK( not std::is_move_constructible< butter::compound_artifact >::type{} );
//XXBOOST_CHECK( not (std::is_assignable< butter::compound_artifact, butter::compound_artifact >::type{}) );
//XXBOOST_CHECK( not std::has_virtual_destructor< butter::compound_artifact >::type{});
//XX

}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( log_lifetime )
{
//XX// Lifetime method tests
//XXBOOST_CHECK( not std::is_default_constructible< butter::log >::type{} );
//XXBOOST_CHECK( not std::is_copy_constructible< butter::log >::type{} );
//XXBOOST_CHECK( not std::is_move_constructible< butter::log >::type{} );
//XXBOOST_CHECK( not (std::is_assignable< butter::log, butter::log >::type{}) );
//XXBOOST_CHECK( not std::has_virtual_destructor< butter::log >::type{});
//XX

}

