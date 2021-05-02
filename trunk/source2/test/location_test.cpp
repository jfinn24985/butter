
#include "location_test.h"
#include "location.h"


#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( location_root_constructor )
{
butter::location loc;
const QString t{ '.' };
BOOST_CHECK_EQUAL(0, loc.children().size());
BOOST_CHECK(t == loc.path());
BOOST_CHECK(t == loc.full_path());
BOOST_CHECK(nullptr == loc.value());
BOOST_CHECK(nullptr == loc.parent());


}

BOOST_AUTO_TEST_CASE( location_child_constructor )
{
std::unique_ptr<butter::location> root{ new butter::location }; 
const QString t{ '.' };
butter::location * loc = new butter::location{ "subpath", *root };

BOOST_CHECK_EQUAL(1, root->children().size());
BOOST_CHECK(t == root->path());
BOOST_CHECK(t == root->full_path());
BOOST_CHECK(nullptr == root->value());
BOOST_CHECK(nullptr == root->parent());

BOOST_CHECK_EQUAL(0, loc->children().size());
BOOST_CHECK("subpath" == loc->path());
BOOST_CHECK("subpath" == loc->full_path());
BOOST_CHECK(nullptr == loc->value());
BOOST_CHECK(root.get() == loc->parent());


}

BOOST_AUTO_TEST_CASE( location_value_method )
{
butter::location loc;
int dpi{ 314 };
const QString t{ '.' };
loc.value(&dpi);
BOOST_CHECK_EQUAL(314, *reinterpret_cast<const int *>(loc.value()));

}

BOOST_AUTO_TEST_CASE( location_set_constructor )
{
{
  butter::location_set loc;
  BOOST_CHECK(loc.source_directory().path() == ".");
  BOOST_CHECK(loc.source_directory().path() == loc.base_path());
  BOOST_CHECK(loc.topdown().empty());
  BOOST_CHECK(nullptr == loc.root());

  //QTextOStream os(stdout);
  //loc.serialize(os);
}
{
  butter::location_set loc("../butter/src");
  BOOST_CHECK(loc.source_directory().path() == "../butter/src");
  BOOST_CHECK(loc.source_directory().path() == loc.base_path());
  BOOST_CHECK(loc.topdown().empty());
  BOOST_CHECK(nullptr == loc.root());

  //QTextOStream os(stdout);
  //loc.serialize(os);
}


}

BOOST_AUTO_TEST_CASE( location_set_insert_method )
{
butter::location_set loc;
loc.insert("path");
BOOST_CHECK(nullptr != loc.root());
BOOST_CHECK("path" == loc.base_path());
BOOST_CHECK(! loc.topdown().empty());

//QTextOStream os(stdout);
//loc.serialize(os);


}

BOOST_AUTO_TEST_CASE( location_set_insert_multiple )
{
butter::location_set loc;
loc.insert("path1");
loc.insert("path1/path2");
loc.insert("path1/path3");
loc.insert("path1/path2/path4");
BOOST_CHECK("path1" == loc.base_path());
BOOST_CHECK_EQUAL(4, loc.topdown().size());

//QTextOStream os(stdout);
//loc.serialize(os);


}

BOOST_AUTO_TEST_CASE( location_set_insert_below_root )
{
butter::location_set loc;
loc.insert("path1/path2/path4");
loc.insert("path1/path2");
loc.insert("path1/path3");
loc.insert("path1");
BOOST_CHECK("path1" == loc.base_path());
BOOST_CHECK_EQUAL(4, loc.topdown().size());

//QTextOStream os(stdout);
//loc.serialize(os);


}

BOOST_AUTO_TEST_CASE( location_set_insert_src_dir )
{
butter::location_set loc("src");
loc.insert("path1");
loc.insert("path1/path2");
loc.insert("path1/path3");
loc.insert("path1/path2/path4");
BOOST_CHECK("src/path1" == loc.base_path());
BOOST_CHECK_EQUAL(4, loc.topdown().size());

//QTextOStream os(stdout);
//loc.serialize(os);


}

BOOST_AUTO_TEST_CASE( location_set_insert_negative_dir )
{
butter::location_set loc("src");
loc.insert("../path0/path1");
loc.insert("../path0/path1/path2");
loc.insert("../path0/path1/path3");
loc.insert("../path0/path1/path2/path4");

BOOST_CHECK("path0/path1" == loc.base_path());
BOOST_CHECK_EQUAL(4, loc.topdown().size());

//QTextOStream os(stdout);
//os << "base_path: " << loc.base_path() << "\n";
//loc.serialize(os);


}

BOOST_AUTO_TEST_CASE( location_set_insert_sibling_root )
{
butter::location_set loc("src");
loc.insert("ax/path1/path2");
loc.insert("ax/path2");
loc.insert("../cat/path3");
loc.insert("dog/path1/path2/path4");

BOOST_CHECK("src/.." == loc.base_path());
BOOST_CHECK_EQUAL(12, loc.topdown().size());

//QTextOStream os(stdout);
//os << "base_path: " << loc.base_path() << "\n";
//loc.serialize(os);


}

BOOST_AUTO_TEST_CASE( location_set_insert_sibling_root_two )
{
butter::location_set loc("src1/src2");
loc.insert("ax/path1/path2");
loc.insert("ax/path2");
loc.insert("cat/path3");
loc.insert("../dog/path1");

BOOST_CHECK("src1" == loc.base_path());
BOOST_CHECK_EQUAL(10, loc.topdown().size());

//QTextOStream os(stdout);
//os << "base_path: " << loc.base_path() << "\n";
//loc.serialize(os);


}

BOOST_AUTO_TEST_CASE( location_set_value_method )
{
butter::location loc;
int dpi{ 314 };
const QString t{ '.' };
loc.value(&dpi);
BOOST_CHECK_EQUAL(314, *reinterpret_cast<const int *>(loc.value()));

}

BOOST_AUTO_TEST_CASE( location_set_find_closest )
{
butter::location_set loc("src");
loc.insert("path1");
loc.insert("path1/path2");
loc.insert("path1/path3");
loc.insert("path1/path2/path4");
BOOST_CHECK("src/path1" == loc.base_path());
BOOST_CHECK_EQUAL(4, loc.topdown().size());

{
  butter::location * cur = loc.find_closest(butter::enumerate_path("path1/path4"));
  BOOST_REQUIRE(nullptr != cur);
  BOOST_CHECK("src/path1" == cur->path());
}
{
  butter::location * cur = loc.find_closest(butter::enumerate_path("path1/path3/path4"));
  BOOST_REQUIRE(nullptr != cur);
  BOOST_CHECK("path3" == cur->path());
}
{
  butter::location * cur = loc.find_closest(butter::enumerate_path("path1/path2/path4"));
  BOOST_REQUIRE(nullptr != cur);
  BOOST_CHECK("path4" == cur->path());
}

//QTextOStream os(stdout);
//loc.serialize(os);


}

