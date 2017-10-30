

#define BOOST_TEST_MODULE core_test
#include <boost/test/unit_test.hpp>

#include "../src/butter/butter_test/core_test_suite.h"
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

// Manuals
#include "version.h"
// -
#include <type_traits>
// - 

/**
 * Test lifetime methods and accessors of values in ctor
 * list.
 */
BOOST_AUTO_TEST_CASE( basic_style_lifetime )
{
// Static Lifetime method tests

// NOTE, not default constructible because it is an abstract class
BOOST_CHECK( not std::is_default_constructible< butter::basic_style >::type{} );
BOOST_CHECK( not std::is_copy_constructible< butter::basic_style >::type{} );
BOOST_CHECK( not std::is_move_constructible< butter::basic_style >::type{} );
BOOST_CHECK( not (std::is_assignable< butter::basic_style, butter::basic_style >::type{}) );
BOOST_CHECK( std::has_virtual_destructor< butter::basic_style >::type{});

{
  QString coms( "/*" );
  QString come( "*/" );
  QString ephrs( "/* END" );
  QString ephre( " */" );
  QString sphrs( "/* START" );
  QString sphre( "*/" );
  QString name( "cmake" );
  auto facty = []()->std::unique_ptr< butter::base_generator >
  { return std::unique_ptr< butter::base_generator >(); };
  // std::unique_ptr< base_generator > (*write_factory_fn_t)()
  butter::basic_style var( coms, come, ephrs, ephre, sphrs, sphre, name, facty );
  BOOST_CHECK_EQUAL( var.comment_string(), coms );
  BOOST_CHECK_EQUAL( var.end_comment_string(), come );
  BOOST_CHECK_EQUAL( var.end_phrase(), ephrs );
  BOOST_CHECK_EQUAL( var.end_end_phrase(), ephre );
  BOOST_CHECK_EQUAL( var.start_phrase(), sphrs );
  BOOST_CHECK_EQUAL( var.end_start_phrase(), sphre );
  BOOST_CHECK_EQUAL( var.name(), name );
}

}

/**
 * Tests "comment_text" and "create_writer"
 */
BOOST_AUTO_TEST_CASE( basic_style_methods )
{
{
  QString coms( "/*" );
  QString come( "*/" );
  QString ephrs( "/* END" );
  QString ephre( " */" );
  QString sphrs( "/* START" );
  QString sphre( "*/" );
  QString name( "cmake" );
  auto facty = []()->std::unique_ptr< butter::base_generator >
  { return std::unique_ptr< butter::base_generator >(); };
  // std::unique_ptr< base_generator > (*write_factory_fn_t)()
  butter::basic_style var( coms, come, ephrs, ephre, sphrs, sphre, name, facty );
  {
    QString message( "a\n b \nc" );
    QString result;
    BOOST_CHECK_NO_THROW( result = var.comment_text( message ) );
  
    BOOST_CHECK_EQUAL( result, "/*a*/\n/* b */\n/*c*/\n" );
    std::unique_ptr< butter::base_generator > p;
    BOOST_CHECK_NO_THROW( p = var.create_writer() );
    BOOST_CHECK( !p );
  }
}

}

/**
 * Tests expected value of butter constants.
 */
BOOST_AUTO_TEST_CASE( butter_constants )
{
BOOST_CHECK_EQUAL( butter::butter_constants::bindir_value, "BINDIR" );
BOOST_CHECK_EQUAL( butter::butter_constants::buildfile_value, "buildfile" );
BOOST_CHECK_EQUAL( butter::butter_constants::BUTTER_VERSION, "Butter version 1.2beta." BUTTER_SVN_VERSION_ );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_base_name, "butter base" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_build_dir_name, "butter build-dir" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_buildfile_name, "butter buildfile" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_compiler_name, "butter compiler" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_flags_name, "butter flags" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_install_name, "butter install" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_ldflags_name, "butter ldflags" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_lib_type_name, "butter type" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_log_name, "butter log-level" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_other_name, "butter other" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_include_name, "butter include" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_project_name, "butter project" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_style_name, "butter style" );
BOOST_CHECK_EQUAL( butter::butter_constants::butter_version_name, "butter version" );
BOOST_CHECK_EQUAL( butter::butter_constants::document_stereotype, "document" );
BOOST_CHECK_EQUAL( butter::butter_constants::executable_stereotype, "executable" );
BOOST_CHECK_EQUAL( butter::butter_constants::flag_label, "FLAGS" );
BOOST_CHECK_EQUAL( butter::butter_constants::generic_section, "generic" );
BOOST_CHECK_EQUAL( butter::butter_constants::header_label, "HDR" );
BOOST_CHECK_EQUAL( butter::butter_constants::libdir_value, "LIBDIR" );
BOOST_CHECK_EQUAL( butter::butter_constants::library_stereotype, "library" );
BOOST_CHECK_EQUAL( butter::butter_constants::link_label, "LINK" );
BOOST_CHECK_EQUAL( butter::butter_constants::no_install_value, "NONE" );
BOOST_CHECK_EQUAL( butter::butter_constants::section_prefix, "butter_" );
BOOST_CHECK_EQUAL( butter::butter_constants::shared_value, "shared" );
BOOST_CHECK_EQUAL( butter::butter_constants::source_stereotype, "source" );
BOOST_CHECK_EQUAL( butter::butter_constants::static_value, "static" );
BOOST_CHECK_EQUAL( butter::butter_constants::version_name, "version" );
BOOST_CHECK_EQUAL( butter::butter_constants::version_label, "version" );
BOOST_CHECK_EQUAL( butter::butter_constants::date_label, "date" );
BOOST_CHECK_EQUAL( butter::butter_constants::preamble_label, "preamble" );
BOOST_CHECK_EQUAL( butter::butter_constants::close_label, "close" );
BOOST_CHECK_EQUAL( butter::butter_constants::target_label, "target" );

#ifdef _WIN32
BOOST_CHECK_EQUAL( butter::butter_constants::app_data_env_var, "APPDATA" );
#else
BOOST_CHECK_EQUAL( butter::butter_constants::app_data_env_var, "HOME" );
#endif

#ifdef _WIN32
BOOST_CHECK_EQUAL( butter::butter_constants::app_data_subdir_name, "Butter" );
#else
BOOST_CHECK_EQUAL( butter::butter_constants::app_data_subdir_name, ".butter" );
#endif


}

/**
 * Test lifetime methods and accessors of values in ctor
 * list.
 */
BOOST_AUTO_TEST_CASE( pathcmp_lifetime )
{
// Static Lifetime method tests

// NOTE, not default constructible because it is an abstract class
BOOST_CHECK( std::is_default_constructible< butter::pathcmp >::type{} );
BOOST_CHECK( std::is_copy_constructible< butter::pathcmp >::type{} );
BOOST_CHECK( std::is_move_constructible< butter::pathcmp >::type{} );
BOOST_CHECK( (std::is_assignable< butter::pathcmp, butter::pathcmp >::type{}) );
BOOST_CHECK( std::has_virtual_destructor< butter::pathcmp >::type{});

{
  QString apath( "./test" );
  butter::pathcmp var( apath );
  BOOST_CHECK_EQUAL( var.depth(), 1 );
  BOOST_CHECK_EQUAL( var.leaf_at( 0 ), "test" );
}

}

/**
 * Test methods and accessors of pathcmp
 * 
 * Untested: mkpath, path_localised
 * 
 * Tested: pathcmp(unistr), op=(unistr) {implicit conversion}
 *   depth, equality, has_subpath, leaf_at, path, setPath,
 *   isRelative, exists, , operator/, pathcmp(QDir),
 *   create_common, create_relative, path_convert
 * 
 * Failed:
 *   create_common, create_relative
 *   
 */
BOOST_AUTO_TEST_CASE( pathcmp_methods )
{
// UNIX style paths
{
  QString a1("/home/is/where/heart/is");
	QString a2("/home/is/where/heart/is/");
	QString a3("/home/is/where");
	QString a4("/home/is/hwere/heart/is");
  butter::pathcmp p1, p2;
  // Assign from string.
  p1 = a1;
  BOOST_CHECK_EQUAL( p1.path(), a1 );
  BOOST_REQUIRE_EQUAL( p1.depth (), 6 );
  BOOST_CHECK_EQUAL( p1.leaf_at( 0 ), "" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 1 ), "home" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 2 ), "is" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 3 ), "where" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 4 ), "heart" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 5 ), "is" );

  // Use setPath
  p2.setPath( a2 );
  BOOST_CHECK_EQUAL( p2.path(), a2.left( a2.length() - 1 ) );
  BOOST_CHECK_EQUAL( p2.depth (), 6 );
  BOOST_CHECK_EQUAL( p2.leaf_at( 0 ), "" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 1 ), "home" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 2 ), "is" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 3 ), "where" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 4 ), "heart" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 5 ), "is" );

  BOOST_CHECK( p1.equality( p2 ) );

  // Construct from string
  butter::pathcmp p3( a3 );
  BOOST_CHECK_EQUAL( p3.path(), a3 );
  BOOST_CHECK_EQUAL( p3.depth (), 4 );
  BOOST_CHECK_EQUAL( p3.leaf_at( 0 ), "" );
  BOOST_CHECK_EQUAL( p3.leaf_at( 1 ), "home" );
  BOOST_CHECK_EQUAL( p3.leaf_at( 2 ), "is" );
  BOOST_CHECK_EQUAL( p3.leaf_at( 3 ), "where" );

  BOOST_CHECK( p3.has_subpath(p1) );

  // reassign p2 from string
  p2 = a4;
  BOOST_CHECK_EQUAL( p2.path(), a4 );
  BOOST_CHECK_EQUAL( p2.depth (), 6 );
  BOOST_CHECK_EQUAL( p2.leaf_at( 0 ), "" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 1 ), "home" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 2 ), "is" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 3 ), "hwere" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 4 ), "heart" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 5 ), "is" );

  BOOST_CHECK( not p1.equality( p2 ) );

  BOOST_CHECK( not p3.has_subpath(p2) );

  BOOST_CHECK_EQUAL( p1.create_common( p3 ), a3 );
  BOOST_CHECK_EQUAL( p3.create_common( p1 ), a3 );
  
  QString common23 = p2.create_common( p3 );
  BOOST_CHECK_EQUAL( common23, "/home/is" );
  BOOST_CHECK_EQUAL( common23, p3.create_common( p2 ) );

  BOOST_CHECK_EQUAL( p3.create_relative( p2 ), "../hwere/heart/is" );
  BOOST_CHECK_EQUAL( p2.create_relative( p3 ), "../../../where" );
  BOOST_CHECK_EQUAL( p3.create_relative( p1 ), "heart/is" );
  BOOST_CHECK_EQUAL( p1.create_relative( p3 ), "../.." );
}
// MS DOS style paths
{
	QString a1("C:\\home\\is\\where\\heart\\is");
	QString a2("C:\\home\\is\\where\\heart\\is\\");
	QString a3("C:\\home\\is\\where");
	QString a4("D:\\home\\is\\where\\heart\\is");
  butter::pathcmp p1, p2;
  // Assign from string.
  p1 = a1;
  BOOST_CHECK_EQUAL( p1.path (), "C:/home/is/where/heart/is" );
  BOOST_REQUIRE_EQUAL( p1.depth (), 6 );
  BOOST_CHECK_EQUAL( p1.leaf_at( 0 ), "C:" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 1 ), "home" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 2 ), "is" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 3 ), "where" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 4 ), "heart" );
  BOOST_CHECK_EQUAL( p1.leaf_at( 5 ), "is" );

  // Use setPath
  p2.setPath( a2 );
  BOOST_CHECK_EQUAL( p2.path (), "C:/home/is/where/heart/is" );
  BOOST_CHECK_EQUAL( p2.depth (), 6 );
  BOOST_CHECK_EQUAL( p2.leaf_at( 0 ), "C:" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 1 ), "home" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 2 ), "is" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 3 ), "where" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 4 ), "heart" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 5 ), "is" );

  BOOST_CHECK( p1.equality( p2 ) );

  // Construct from string
  butter::pathcmp p3( a3 );
  BOOST_CHECK_EQUAL( p3.path (), "C:/home/is/where" );
  BOOST_CHECK_EQUAL( p3.depth (), 4 );
  BOOST_CHECK_EQUAL( p3.leaf_at( 0 ), "C:" );
  BOOST_CHECK_EQUAL( p3.leaf_at( 1 ), "home" );
  BOOST_CHECK_EQUAL( p3.leaf_at( 2 ), "is" );
  BOOST_CHECK_EQUAL( p3.leaf_at( 3 ), "where" );

  BOOST_CHECK( p3.has_subpath(p1) );

  // reassign p2 from string
  p2 = a4;
  BOOST_CHECK_EQUAL( p2.path (), "D:/home/is/where/heart/is" );
  BOOST_CHECK_EQUAL( p2.depth (), 6 );
  BOOST_CHECK_EQUAL( p2.leaf_at( 0 ), "D:" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 1 ), "home" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 2 ), "is" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 3 ), "where" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 4 ), "heart" );
  BOOST_CHECK_EQUAL( p2.leaf_at( 5 ), "is" );

  BOOST_CHECK( not p1.equality( p2 ) );

  BOOST_CHECK( not p3.has_subpath(p2) );

  BOOST_CHECK_EQUAL( p1.create_common( p3 ), a3 );
  BOOST_CHECK_EQUAL( p3.create_common( p1 ), a3 );
  
  QString common23 = p2.create_common( p3 );
  BOOST_CHECK_EQUAL( common23, "C:/home/is" );
  BOOST_CHECK_EQUAL( common23, p3.create_common( p2 ) );

  BOOST_CHECK_EQUAL( p3.create_relative( p2 ), "../hwere/heart/is" );
  BOOST_CHECK_EQUAL( p2.create_relative( p3 ), "../../../where" );
  BOOST_CHECK_EQUAL( p3.create_relative( p1 ), "heart/is" );
  BOOST_CHECK_EQUAL( p1.create_relative( p3 ), "../.." );
}
{
  QDir qp( "../ace" );
  butter::pathcmp p1( qp );
  butter::pathcmp p2( "leaf" );
  BOOST_CHECK_EQUAL( qp.path(), p1.path() );
  BOOST_CHECK( p1.isRelative() );
  BOOST_CHECK( not p1.exists() );
  butter::pathcmp p3( p1 / p2 / "end" );
  BOOST_CHECK_EQUAL( p3.path(), "../ace/leaf/end" );
  QString s3( p3.path_convert( "\\" ) );
  BOOST_CHECK_EQUAL( s3, "..\\ace\\leaf\\end" );
}

}

/**
 * Test lifetime methods and static non-bouml methods
 * 
 * Static methods to test:
 *   section
 */
BOOST_AUTO_TEST_CASE( base_generator_lifetime )
{
// Static Lifetime method tests

// Ctor public but cannot create as class is abstract
BOOST_CHECK( not std::is_default_constructible< butter::base_generator >::type{} );
BOOST_CHECK( not std::is_copy_constructible< butter::base_generator >::type{} );
BOOST_CHECK( not std::is_move_constructible< butter::base_generator >::type{} );
BOOST_CHECK( not (std::is_assignable< butter::base_generator, butter::base_generator >::type{}) );
BOOST_CHECK( std::has_virtual_destructor< butter::base_generator >::type{});



}

/**
 * Test static non-bouml method
 */
BOOST_AUTO_TEST_CASE( base_gen_merge_string_list )
{
// Static method tests
{
  QString a("a b c");
  QString b("d e");
  QString result("a b c d e");
  butter::base_generator::merge_string_list( a, b );
  BOOST_CHECK_EQUAL( a, result );
}
{
  QString a("a b c");
  QString b("c d e");
  QString result("a b c d e");
  butter::base_generator::merge_string_list( a, b );
  BOOST_CHECK_EQUAL( a, result );
}
{
  QString a("c d e");
  QString b("a b c");
  QString result("c d e a b");
  butter::base_generator::merge_string_list( a, b );
  BOOST_CHECK_EQUAL( a, result );
}
{
  QString a("c d e");
  QString b("a 'b c'");
  QString result("c d e a 'b c'");
  butter::base_generator::merge_string_list( a, b );
  BOOST_CHECK_EQUAL( a, result );
}
{
  QString a("c d e");
  QString b("a \"b c\"");
  QString result("c d e a \"b c\"");
  butter::base_generator::merge_string_list( a, b );
  BOOST_CHECK_EQUAL( a, result );
}
{
  QString a("\"a c\" d e");
  QString b("a \"b c\"");
  QString result("\"a c\" d e a \"b c\"");
  butter::base_generator::merge_string_list( a, b );
  BOOST_CHECK_EQUAL( a, result );
}

}

/**
 * Test static non-bouml method
 */
BOOST_AUTO_TEST_CASE( base_gen_section )
{
//XX// Static method tests
//XX{
//XXQString a("a b c d e");
//XXQString result("a f c d e");
//XXbutter::base_generator::find_replace( a, 'b', 'f' );
//XXBOOST_CHECK_EQUAL( a, result );
//XX}
//XX{
//XXQString a("a b c");
//XXQString result("c b c");
//XXbutter::base_generator::find_replace( a, 'a', 'c' );
//XXBOOST_CHECK_EQUAL( a, result );
//XX}{
//XXQString a("a \"b c\"");
//XXQString result("a \"b d\"");
//XXbutter::base_generator::find_replace( a, 'c', 'd' );
//XXBOOST_CHECK_EQUAL( a, result );
//XX}
//XX{
//XXQString a("\'a c\' d e");
//XXQString result("\'a c\' e e");
//XXbutter::base_generator::find_replace( a, 'd', 'e' );
//XXBOOST_CHECK_EQUAL( a, result );
//XX}
//XX{
//XXQString a("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabacadaea");
//XXQString result("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffbfcfdfef");
//XXbutter::base_generator::find_replace( a, 'a', 'f' );
//XXBOOST_CHECK_EQUAL( a, result );
//XX}

}

/**
 * Test lifetime methods and accessors of values in ctor
 * list.
 */
BOOST_AUTO_TEST_CASE( location_lifetime )
{
// Lifetime method tests
BOOST_CHECK( not std::is_default_constructible< butter::location >::type{} );
BOOST_CHECK( not std::is_copy_constructible< butter::location >::type{} );
BOOST_CHECK( not std::is_move_constructible< butter::location >::type{} );
BOOST_CHECK( not (std::is_assignable< butter::location, butter::location >::type{}) );
BOOST_CHECK( not std::has_virtual_destructor< butter::location >::type{});

butter::pathcmp p1( "./src/test" );
butter::location l1( p1 );

BOOST_CHECK_EQUAL( l1.path().path(), p1.path() );
BOOST_CHECK( l1.parent() == nullptr );
BOOST_CHECK( l1.packages().isEmpty() );
BOOST_CHECK_EQUAL( l1.packages().count(), 0 );
BOOST_CHECK_EQUAL( l1.packages().size(), 0 );
BOOST_CHECK( l1.children().isEmpty() );
BOOST_CHECK_EQUAL( l1.children().count(), 0 );
BOOST_CHECK_EQUAL( l1.children().size(), 0 );

BOOST_CHECK_EQUAL( l1.full_path().path(), p1.path() );


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
butter::pathcmp base_path( "./src/test" );
butter::location root( base_path );

BOOST_CHECK_EQUAL( root.path().path(), base_path.path() );
BOOST_CHECK( root.parent() == nullptr );
BOOST_CHECK( root.packages().isEmpty() );
BOOST_CHECK_EQUAL( root.packages().size(), 0 );
BOOST_CHECK_EQUAL( root.packages().count(), 0 );
BOOST_CHECK( root.children().isEmpty() );

BOOST_CHECK_EQUAL( root.full_path().path(), base_path.path() );

butter::pathcmp leaf_path1( "src/test/subdir1" );
butter::location * child1 = root.create_as_child( leaf_path1 );

BOOST_CHECK_EQUAL( child1->path().path(), "subdir1" );
BOOST_CHECK_EQUAL( child1->full_path().path(), "src/test/subdir1" );
BOOST_CHECK( child1->parent() != nullptr );
BOOST_CHECK( child1->packages().isEmpty() );
BOOST_CHECK_EQUAL( child1->packages().size(), 0 );
BOOST_CHECK_EQUAL( child1->packages().count(), 0 );
BOOST_CHECK( child1->children().isEmpty() );
BOOST_CHECK_EQUAL( child1->children().size(), 0 );
BOOST_CHECK_EQUAL( child1->children().count(), 0 );
BOOST_CHECK((not root.children().isEmpty()) );
BOOST_CHECK_EQUAL( root.children().size(), 1 );
BOOST_CHECK_EQUAL( root.children().count(), 1 );
BOOST_CHECK( &root == child1->parent() );

butter::pathcmp leaf_path2( "./src/test/subdir2" );
butter::location * child2 = root.create_as_child( leaf_path2 );

BOOST_CHECK_EQUAL( child2->path().path(), "subdir2" );
BOOST_CHECK_EQUAL( child2->full_path().path(), "src/test/subdir2" );
BOOST_CHECK( child2->parent() != nullptr );
BOOST_CHECK( child2->packages().isEmpty() );
BOOST_CHECK_EQUAL( child2->packages().size(), 0 );
BOOST_CHECK_EQUAL( child2->packages().count(), 0 );
BOOST_CHECK( child2->children().isEmpty() );
BOOST_CHECK_EQUAL( child2->children().size(), 0 );
BOOST_CHECK_EQUAL( child2->children().count(), 0 );
BOOST_CHECK( (not root.children().isEmpty()) );
BOOST_CHECK_EQUAL( root.children().size(), 2 );
BOOST_CHECK_EQUAL( root.children().count(), 2 );
BOOST_CHECK( &root == child2->parent() );

if (DEBUG)
{
  QString result;
  QTextOStream qos( &result );
  root.serialize( qos );
  QString canon( "<<BEGIN location [src/test]\nChildren:\n[0] = subdir1\n[1] = subdir2\n<<END location [src/test]\n<<BEGIN location [src/test/subdir1]\n<<END location [src/test/subdir1]\n<<BEGIN location [src/test/subdir2]\n<<END location [src/test/subdir2]\n" );
  BOOST_CHECK_EQUAL( result, canon );

}

}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( bjam_generator_lifetime )
{
// Lifetime method tests
BOOST_CHECK( not std::is_default_constructible< butter::bjam_generator >::type{} );
BOOST_CHECK( not std::is_copy_constructible< butter::bjam_generator >::type{} );
BOOST_CHECK( not std::is_move_constructible< butter::bjam_generator >::type{} );
BOOST_CHECK( not (std::is_assignable< butter::bjam_generator, butter::bjam_generator >::type{}) );
BOOST_CHECK( std::has_virtual_destructor< butter::bjam_generator >::type{});

// General variables
BOOST_CHECK_EQUAL( butter::bjam_generator::build_file_name, "Jamfile" );
BOOST_CHECK_EQUAL( butter::bjam_generator::build_file_sysname, "Jamroot" );
BOOST_CHECK_EQUAL( butter::bjam_generator::rules_name, "local.jam" );
// BOOST_CHECK_EQUAL( butter::bjam_generator::default_rules, "Jamfile" );

// style
BOOST_CHECK_EQUAL( butter::bjam_generator::style.comment_string(), "#" );
BOOST_CHECK_EQUAL( butter::bjam_generator::style.end_comment_string(), "" );
BOOST_CHECK_EQUAL( butter::bjam_generator::style.end_phrase(), "##END:" );
BOOST_CHECK_EQUAL( butter::bjam_generator::style.end_end_phrase(), "" );
BOOST_CHECK_EQUAL( butter::bjam_generator::style.start_phrase(), "##START:" );
BOOST_CHECK_EQUAL( butter::bjam_generator::style.end_start_phrase(), "" );
BOOST_CHECK_EQUAL( butter::bjam_generator::style.name(), "boost" );

auto pgen = butter::bjam_generator::create();
BOOST_CHECK( pgen );
BOOST_CHECK( nullptr != pgen.get() );

BOOST_CHECK_EQUAL( pgen->target_NAME(), "" );
BOOST_CHECK_EQUAL( pgen->root_dir().path(), "." );

}

/**
 * Test lifetime methods and static attributes
 */
BOOST_AUTO_TEST_CASE( cmake_generator_lifetime )
{
// Lifetime method tests
BOOST_CHECK( not std::is_default_constructible< butter::cmake_generator >::type{} );
BOOST_CHECK( not std::is_copy_constructible< butter::cmake_generator >::type{} );
BOOST_CHECK( not std::is_move_constructible< butter::cmake_generator >::type{} );
BOOST_CHECK( not (std::is_assignable< butter::cmake_generator, butter::cmake_generator >::type{}) );
BOOST_CHECK( std::has_virtual_destructor< butter::cmake_generator >::type{});

// Common values.
BOOST_CHECK_EQUAL( butter::cmake_generator::build_file_name, "CMakeLists.txt" );
BOOST_CHECK_EQUAL( butter::cmake_generator::build_file_sysname, "CMakeLists.txt" );
BOOST_CHECK_EQUAL( butter::cmake_generator::rules_name, "local.cmake" );
// BOOST_CHECK_EQUAL( butter::cmake_generator::default_rules, "Jamfile" );

// CMAKE specific
BOOST_CHECK_EQUAL( butter::cmake_generator::cmake_minimum_required_, "cmake_minimum_required(VERSION 2.6)" );

// Jam style
BOOST_CHECK_EQUAL( butter::cmake_generator::style.comment_string(), "#" );
BOOST_CHECK_EQUAL( butter::cmake_generator::style.end_comment_string(), "" );
BOOST_CHECK_EQUAL( butter::cmake_generator::style.end_phrase(), "##END:" );
BOOST_CHECK_EQUAL( butter::cmake_generator::style.end_end_phrase(), "" );
BOOST_CHECK_EQUAL( butter::cmake_generator::style.start_phrase(), "##START:" );
BOOST_CHECK_EQUAL( butter::cmake_generator::style.end_start_phrase(), "" );
BOOST_CHECK_EQUAL( butter::cmake_generator::style.name(), "cmake" );

auto pgen = butter::bjam_generator::create();
BOOST_CHECK( pgen );
BOOST_CHECK( nullptr != pgen.get() );

BOOST_CHECK_EQUAL( pgen->target_NAME(), "" );
BOOST_CHECK_EQUAL( pgen->root_dir().path(), "." );

}

/**
 * Test lifetime methods and static attributes
 */
BOOST_AUTO_TEST_CASE( gmake_generator_lifetime )
{
// Lifetime method tests
BOOST_CHECK( not std::is_default_constructible< butter::gmake_generator >::type{} );
BOOST_CHECK( not std::is_copy_constructible< butter::gmake_generator >::type{} );
BOOST_CHECK( not std::is_move_constructible< butter::gmake_generator >::type{} );
BOOST_CHECK( not (std::is_assignable< butter::gmake_generator, butter::gmake_generator >::type{}) );
BOOST_CHECK( std::has_virtual_destructor< butter::gmake_generator >::type{});

// Common values.
BOOST_CHECK_EQUAL( butter::gmake_generator::build_file_name, "makefile" );
BOOST_CHECK_EQUAL( butter::gmake_generator::build_file_sysname, "makefile" );
BOOST_CHECK_EQUAL( butter::gmake_generator::rules_name, "M_sys.mk M_cl.mk M_gcc.mk M_unix.mk M_Windows_NT.mk" );
// BOOST_CHECK_EQUAL( butter::gmake_generator::default_rules, "Jamfile" );

// Gnu make specific
// BOOST_CHECK_EQUAL( butter::gmake_generator::default_rules_sys, "" );

// style
BOOST_CHECK_EQUAL( butter::gmake_generator::style.comment_string(), "#" );
BOOST_CHECK_EQUAL( butter::gmake_generator::style.end_comment_string(), "" );
BOOST_CHECK_EQUAL( butter::gmake_generator::style.end_phrase(), "##END:" );
BOOST_CHECK_EQUAL( butter::gmake_generator::style.end_end_phrase(), "" );
BOOST_CHECK_EQUAL( butter::gmake_generator::style.start_phrase(), "##START:" );
BOOST_CHECK_EQUAL( butter::gmake_generator::style.end_start_phrase(), "" );
BOOST_CHECK_EQUAL( butter::gmake_generator::style.name(), "make" );

auto pgen = butter::bjam_generator::create();
BOOST_CHECK( pgen );
BOOST_CHECK( nullptr != pgen.get() );

BOOST_CHECK_EQUAL( pgen->target_NAME(), "" );
BOOST_CHECK_EQUAL( pgen->root_dir().path(), "." );

}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( jam_generator_lifetime )
{
// Lifetime method tests
BOOST_CHECK( not std::is_default_constructible< butter::jam_generator >::type{} );
BOOST_CHECK( not std::is_copy_constructible< butter::jam_generator >::type{} );
BOOST_CHECK( not std::is_move_constructible< butter::jam_generator >::type{} );
BOOST_CHECK( not (std::is_assignable< butter::jam_generator, butter::jam_generator >::type{}) );
BOOST_CHECK( std::has_virtual_destructor< butter::jam_generator >::type{});

// General variables.
BOOST_CHECK_EQUAL( butter::jam_generator::build_file_name, "Jamfile" );
BOOST_CHECK_EQUAL( butter::jam_generator::build_file_sysname, "Jamfile" );
BOOST_CHECK_EQUAL( butter::jam_generator::rules_name, "Jamrules" );
// BOOST_CHECK_EQUAL( butter::jam_generator::default_rules, "Jamfile" );

// Jam style
BOOST_CHECK_EQUAL( butter::jam_generator::style.comment_string(), "#" );
BOOST_CHECK_EQUAL( butter::jam_generator::style.end_comment_string(), "" );
BOOST_CHECK_EQUAL( butter::jam_generator::style.end_phrase(), "##END:" );
BOOST_CHECK_EQUAL( butter::jam_generator::style.end_end_phrase(), "" );
BOOST_CHECK_EQUAL( butter::jam_generator::style.start_phrase(), "##START:" );
BOOST_CHECK_EQUAL( butter::jam_generator::style.end_start_phrase(), "" );
BOOST_CHECK_EQUAL( butter::jam_generator::style.name(), "standard" );

auto pgen = butter::bjam_generator::create();
BOOST_CHECK( pgen );
BOOST_CHECK( nullptr != pgen.get() );

BOOST_CHECK_EQUAL( pgen->target_NAME(), "" );
BOOST_CHECK_EQUAL( pgen->root_dir().path(), "." );

}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( compound_artifact_lifetime )
{
// Lifetime method tests
BOOST_CHECK( not std::is_default_constructible< butter::compound_artifact >::type{} );
BOOST_CHECK( not std::is_copy_constructible< butter::compound_artifact >::type{} );
BOOST_CHECK( not std::is_move_constructible< butter::compound_artifact >::type{} );
BOOST_CHECK( not (std::is_assignable< butter::compound_artifact, butter::compound_artifact >::type{}) );
BOOST_CHECK( not std::has_virtual_destructor< butter::compound_artifact >::type{});


}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( const_token_iterator_lifetime )
{
// Lifetime method tests
BOOST_CHECK( std::is_default_constructible< butter::const_token_iterator >::type{} );
BOOST_CHECK( std::is_copy_constructible< butter::const_token_iterator >::type{} );
BOOST_CHECK( std::is_move_constructible< butter::const_token_iterator >::type{} );
BOOST_CHECK( (std::is_assignable< butter::const_token_iterator, butter::const_token_iterator >::type{}) );
BOOST_CHECK( not std::has_virtual_destructor< butter::const_token_iterator >::type{});


}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( const_token_iterator_method )
{
// Iterator method tests
{
  QString a("a b c d e");
  QString b[5]{ {"a"}, {"b"}, {"c"}, {"d"}, {"e"} };
  butter::const_token_iterator itr( a, {' '} );
  for( unsigned int ii = 0; ii != 5; ++itr, ++ii )
  {
    BOOST_CHECK( *itr == b[ii] );
  }
}
{
  QString a("\'a b\' b c d e");
  QString b[5]{ {"\'a b\'"}, {"b"}, {"c"}, {"d"}, {"e"} };
  butter::const_token_iterator itr( a, {' '} );
  for( unsigned int ii = 0; ii != 5; ++itr, ++ii )
  {
    BOOST_CHECK( *itr == b[ii] );
  }
}
{
  QString a("\"a b\" b c d e");
  QString b[5]{ {"\"a b\""}, {"b"}, {"c"}, {"d"}, {"e"} };
  butter::const_token_iterator itr( a, {' '} );
  for( unsigned int ii = 0; ii != 5; ++itr, ++ii )
  {
    BOOST_CHECK( *itr == b[ii] );
  }
}
{
  QString a("a\tb b c d e");
  QString b[5]{ {"a\tb"}, {"b"}, {"c"}, {"d"}, {"e"} };
  butter::const_token_iterator itr( a, {' '} );
  for( unsigned int ii = 0; ii != 5; ++itr, ++ii )
  {
    BOOST_CHECK( *itr == b[ii] );
  }
}
{
  QString a("a,b, c, d, e");
  QString b[5]{ {"a"}, {"b"}, {" c"}, {" d"}, {" e"} };
  butter::const_token_iterator itr( a, {','} );
  for( unsigned int ii = 0; ii != 5; ++itr, ++ii )
  {
    BOOST_CHECK( *itr == b[ii] );
  }
}

}

/**
 * Test lifetime methods and static attributes
 * list.
 */
BOOST_AUTO_TEST_CASE( log_lifetime )
{
// Lifetime method tests
BOOST_CHECK( not std::is_default_constructible< butter::log >::type{} );
BOOST_CHECK( not std::is_copy_constructible< butter::log >::type{} );
BOOST_CHECK( not std::is_move_constructible< butter::log >::type{} );
BOOST_CHECK( not (std::is_assignable< butter::log, butter::log >::type{}) );
BOOST_CHECK( not std::has_virtual_destructor< butter::log >::type{});


}

