
#include "compound_document_test.h"
#include "compound_document.h"


#define BOOST_TEST_DYN_LINK // only in one file
#define BOOST_TEST_MODULE butter2
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( compdoc_empty_constructor )
{
butter::compound_document doc;
BOOST_CHECK_EQUAL(0, doc.size());
BOOST_CHECK(doc.empty());
BOOST_CHECK(doc.close().isEmpty());
BOOST_CHECK(doc.date().isEmpty());
BOOST_CHECK(doc.end_phrase().isEmpty());
BOOST_CHECK(doc.postscript().isEmpty());
BOOST_CHECK(doc.preamble().isEmpty());
BOOST_CHECK(doc.start_phrase().isEmpty());
BOOST_CHECK(doc.version().isEmpty());


}

BOOST_AUTO_TEST_CASE( compdoc_access )
{
butter::compound_document doc;

doc["element"].value = "Some value\n";
doc["element"].user = "Some user text\n";
doc.close().user = "Close user\n";
doc.close().value = "Close value\n";
doc.date().user = "Date user\n";
doc.date().value = "Date value\n";
doc.preamble().user = "preamble user\n";
doc.preamble().value = "preamble value\n";
doc.version().user = "version user\n";
doc.version().value = "version value\n";
doc.postscript() = "postscript\n";
doc.end_phrase( "##END" );
doc.start_phrase( "##START" );


BOOST_CHECK(doc.has_target("element"));
BOOST_CHECK(!doc.has_target("element2"));
BOOST_CHECK(!doc.empty());
BOOST_CHECK_EQUAL(1, doc.size());
BOOST_CHECK(doc["element"].label == "element");
BOOST_CHECK(doc["element"].value == "Some value\n");
BOOST_CHECK(doc["element"].user == "Some user text\n");
BOOST_CHECK(doc.close().value == "Close value\n");
BOOST_CHECK(doc.close().user == "Close user\n");
BOOST_CHECK(doc.date().value == "Date value\n");
BOOST_CHECK(doc.date().user == "Date user\n");
BOOST_CHECK(doc.preamble().value == "preamble value\n");
BOOST_CHECK(doc.preamble().user == "preamble user\n");
BOOST_CHECK(doc.version().value == "version value\n");
BOOST_CHECK(doc.version().user == "version user\n");
BOOST_CHECK(doc.postscript() == "postscript\n");
BOOST_CHECK(doc.end_phrase() == "##END");
BOOST_CHECK(doc.start_phrase() == "##START");


}

BOOST_AUTO_TEST_CASE( compdoc_copy_constructor )
{
butter::compound_document doc;

doc["element"].value = "Some value\n";
doc["element"].user = "Some user text\n";
doc.close().user = "Close user\n";
doc.close().value = "Close value\n";
doc.date().user = "Date user\n";
doc.date().value = "Date value\n";
doc.preamble().user = "preamble user\n";
doc.preamble().value = "preamble value\n";
doc.version().user = "version user\n";
doc.version().value = "version value\n";
doc.postscript() = "postscript\n";
doc.end_phrase( "##END" );
doc.start_phrase( "##START" );

butter::compound_document doc1(doc);

BOOST_CHECK(!doc1.empty());
BOOST_CHECK(!doc1.has_target("element2"));
BOOST_CHECK(doc1.close().user == "Close user\n");
BOOST_CHECK(doc1.close().value == "Close value\n");
BOOST_CHECK(doc1.date().user == "Date user\n");
BOOST_CHECK(doc1.date().value == "Date value\n");
BOOST_CHECK(doc1.has_target("element"));
BOOST_CHECK(doc1.postscript() == "postscript\n");
BOOST_CHECK(doc1.preamble().user == "preamble user\n");
BOOST_CHECK(doc1.preamble().value == "preamble value\n");
BOOST_CHECK(doc1.version().user == "version user\n");
BOOST_CHECK(doc1.version().value == "version value\n");
BOOST_CHECK(doc1["element"].label == "element");
BOOST_CHECK(doc1["element"].user == "Some user text\n");
BOOST_CHECK(doc1["element"].value == "Some value\n");
BOOST_CHECK_EQUAL(1, doc1.size());
BOOST_CHECK(doc1.end_phrase() == "##END");
BOOST_CHECK(doc1.start_phrase() == "##START");

}

BOOST_AUTO_TEST_CASE( compdoc_assignment )
{
butter::compound_document doc;

doc["element"].value = "Some value\n";
doc["element"].user = "Some user text\n";
doc.close().user = "Close user\n";
doc.close().value = "Close value\n";
doc.date().user = "Date user\n";
doc.date().value = "Date value\n";
doc.preamble().user = "preamble user\n";
doc.preamble().value = "preamble value\n";
doc.version().user = "version user\n";
doc.version().value = "version value\n";
doc.postscript() = "postscript\n";
doc.end_phrase( "##END" );
doc.start_phrase( "##START" );

butter::compound_document doc1;
doc1["element2"].user = "Some user2 text\n";
doc1.close().user = "Close user2\n";
BOOST_CHECK(doc1.has_target("element2"));

doc1 = doc;

BOOST_CHECK(!doc1.empty());
BOOST_CHECK(!doc1.has_target("element2"));
BOOST_CHECK(doc1.close().user == "Close user\n");
BOOST_CHECK(doc1.close().value == "Close value\n");
BOOST_CHECK(doc1.date().user == "Date user\n");
BOOST_CHECK(doc1.date().value == "Date value\n");
BOOST_CHECK(doc1.has_target("element"));
BOOST_CHECK(doc1.postscript() == "postscript\n");
BOOST_CHECK(doc1.preamble().user == "preamble user\n");
BOOST_CHECK(doc1.preamble().value == "preamble value\n");
BOOST_CHECK(doc1.version().user == "version user\n");
BOOST_CHECK(doc1.version().value == "version value\n");
BOOST_CHECK(doc1["element"].label == "element");
BOOST_CHECK(doc1["element"].user == "Some user text\n");
BOOST_CHECK(doc1["element"].value == "Some value\n");
BOOST_CHECK_EQUAL(1, doc1.size());
BOOST_CHECK(doc1.end_phrase() == "##END");
BOOST_CHECK(doc1.start_phrase() == "##START");

}

BOOST_AUTO_TEST_CASE( compdoc_move )
{
butter::compound_document doc;

doc["element"].value = "Some value\n";
doc["element"].user = "Some user text\n";
doc.close().user = "Close user\n";
doc.close().value = "Close value\n";
doc.date().user = "Date user\n";
doc.date().value = "Date value\n";
doc.preamble().user = "preamble user\n";
doc.preamble().value = "preamble value\n";
doc.version().user = "version user\n";
doc.version().value = "version value\n";
doc.postscript() = "postscript\n";
doc.end_phrase( "##END" );
doc.start_phrase( "##START" );

butter::compound_document doc1(std::move(doc));

BOOST_CHECK(!doc1.empty());
BOOST_CHECK(!doc1.has_target("element2"));
BOOST_CHECK(doc1.close().user == "Close user\n");
BOOST_CHECK(doc1.close().value == "Close value\n");
BOOST_CHECK(doc1.date().user == "Date user\n");
BOOST_CHECK(doc1.date().value == "Date value\n");
BOOST_CHECK(doc1.has_target("element"));
BOOST_CHECK(doc1.postscript() == "postscript\n");
BOOST_CHECK(doc1.preamble().user == "preamble user\n");
BOOST_CHECK(doc1.preamble().value == "preamble value\n");
BOOST_CHECK(doc1.version().user == "version user\n");
BOOST_CHECK(doc1.version().value == "version value\n");
BOOST_CHECK(doc1["element"].label == "element");
BOOST_CHECK(doc1["element"].user == "Some user text\n");
BOOST_CHECK(doc1["element"].value == "Some value\n");
BOOST_CHECK_EQUAL(1, doc1.size());
BOOST_CHECK(doc1.end_phrase() == "##END");
BOOST_CHECK(doc1.start_phrase() == "##START");

}

BOOST_AUTO_TEST_CASE( compdoc_merge )
{
butter::compound_document doc;

doc["element"].value = "Some value\n";
doc["element"].user = "Some user text\n";
doc["element1"].value = "Some1 value\n";
doc["element1"].user = "Some1 user text\n";
doc.close().user = "Close user\n";
doc.close().value = "Close value\n";
doc.date().user = "Date user\n";
doc.date().value = "Date value\n";
doc.preamble().user = "preamble user\n";
doc.preamble().value = "preamble value\n";
doc.version().user = "version user\n";
doc.version().value = "version value\n";
doc.postscript() = "postscript\n";
doc.end_phrase( "##END" );
doc.start_phrase( "##START" );

butter::compound_document doc1;
doc1["element"].value = "Some0 value1\n";
doc1["element"].user = "Some0 user text1\n";
doc1["element2"].value = "Some2 value1\n";
doc1["element2"].user = "Some2 user text1\n";
doc1.close().user = "Close user1\n";
doc1.close().value = "Close value1\n";
doc1.date().user = "Date user1\n";
doc1.date().value = "Date value1\n";
doc1.preamble().user = "preamble user1\n";
doc1.preamble().value = "preamble value1\n";
doc1.version().user = "version user1\n";
doc1.version().value = "version value1\n";
doc1.postscript() = "postscript1\n";
doc1.end_phrase( "##END" );
doc1.start_phrase( "##START" );

doc.merge(doc1);
BOOST_CHECK(!doc1.empty());
BOOST_CHECK(doc1.has_target("element"));
BOOST_CHECK(doc1.has_target("element2"));
BOOST_CHECK(doc1.close().user == "Close user1\n");
BOOST_CHECK(doc1.close().value == "Close value1\n");
BOOST_CHECK(doc1.date().user == "Date user1\n");
BOOST_CHECK(doc1.date().value == "Date value1\n");
BOOST_CHECK(doc1.has_target("element"));
BOOST_CHECK(doc1.postscript() == "postscript1\n");
BOOST_CHECK(doc1.preamble().user == "preamble user1\n");
BOOST_CHECK(doc1.preamble().value == "preamble value1\n");
BOOST_CHECK(doc1.version().user == "version user1\n");
BOOST_CHECK(doc1.version().value == "version value1\n");
BOOST_CHECK(doc1["element"].label == "element");
BOOST_CHECK(doc1["element"].user == "Some0 user text1\n");
BOOST_CHECK(doc1["element"].value == "Some0 value1\n");
BOOST_CHECK(doc1["element2"].user == "Some2 user text1\n");
BOOST_CHECK(doc1["element2"].value == "Some2 value1\n");

BOOST_REQUIRE_EQUAL(3, doc.size());
BOOST_CHECK(doc.has_target("element"));
BOOST_CHECK(doc.has_target("element1"));
BOOST_CHECK(doc.has_target("element2"));
BOOST_CHECK(doc.close().user == "Close user\nClose user1\n");
BOOST_CHECK(doc.close().value == "Close value\nClose value1\n");
BOOST_CHECK(doc.date().user == "Date user\nDate user1\n");
BOOST_CHECK(doc.date().value == "Date value\nDate value1\n");
BOOST_CHECK(doc.postscript() == "postscript\npostscript1\n");
BOOST_CHECK(doc.preamble().user == "preamble user\npreamble user1\n");
BOOST_CHECK(doc.preamble().value == "preamble value\npreamble value1\n");
BOOST_CHECK(doc.version().user == "version user\nversion user1\n");
BOOST_CHECK(doc.version().value == "version value\nversion value1\n");
BOOST_CHECK(doc["element"].user == "Some user text\nSome0 user text1\n");
BOOST_CHECK(doc["element"].value == "Some value\nSome0 value1\n");
BOOST_CHECK(doc["element1"].user == "Some1 user text\n");
BOOST_CHECK(doc["element1"].value == "Some1 value\n");
BOOST_CHECK(doc["element2"].user == "Some2 user text1\n");
BOOST_CHECK(doc["element2"].value == "Some2 value1\n");
BOOST_CHECK(doc.end_phrase() == "##END");
BOOST_CHECK(doc.start_phrase() == "##START");


}

BOOST_AUTO_TEST_CASE( compdoc_parse )
{
butter::compound_document doc;

QString canon{ "version user\n## BEGIN version\nversion value\n## END version\nDate user\n## BEGIN date\nDate value\n## END date\npreamble user\n## BEGIN preamble\npreamble value\n## END preamble\nSome user text\n## BEGIN target element\nSome value\n## END target element\nClose user\n## BEGIN close\nClose value\n## END close\npostscript\n" };

QTextIStream iss(&canon);

doc.start_phrase( "## BEGIN " );
doc.end_phrase( "## END " );
doc.parse( iss );


BOOST_CHECK(doc["element"].value == "Some value\n");
BOOST_CHECK(doc["element"].user == "Some user text\n");
BOOST_CHECK(doc.close().user == "Close user\n");
BOOST_CHECK(doc.close().value == "Close value\n");
BOOST_CHECK(doc.date().user == "Date user\n");
BOOST_CHECK(doc.date().value == "Date value\n");
BOOST_CHECK(doc.preamble().user == "preamble user\n");
BOOST_CHECK(doc.preamble().value == "preamble value\n");
BOOST_CHECK(doc.version().user == "version user\n");
BOOST_CHECK(doc.version().value == "version value\n");
BOOST_CHECK(doc.postscript() == "postscript\n");

}

BOOST_AUTO_TEST_CASE( compdoc_reset )
{
butter::compound_document doc;

doc["element"].value = "Some value\n";
doc["element"].user = "Some user text\n";
doc.close().user = "Close user\n";
doc.close().value = "Close value\n";
doc.date().user = "Date user\n";
doc.date().value = "Date value\n";
doc.preamble().user = "preamble user\n";
doc.preamble().value = "preamble value\n";
doc.version().user = "version user\n";
doc.version().value = "version value\n";
doc.postscript() = "postscript\n";

doc.reset();

BOOST_CHECK(doc["element"].label == "element");
BOOST_CHECK(doc["element"].value.isEmpty());
BOOST_CHECK(doc["element"].user == "Some user text\n");
BOOST_CHECK(doc.close().value.isEmpty());
BOOST_CHECK(doc.close().user == "Close user\n");
BOOST_CHECK(doc.date().value == "Date value\n");
BOOST_CHECK(doc.date().user == "Date user\n");
BOOST_CHECK(doc.preamble().value.isEmpty());
BOOST_CHECK(doc.preamble().user == "preamble user\n");
BOOST_CHECK(doc.version().value == "version value\n");
BOOST_CHECK(doc.version().user == "version user\n");
BOOST_CHECK(doc.postscript() == "postscript\n");


}

BOOST_AUTO_TEST_CASE( compdoc_write )
{
butter::compound_document doc;

doc["element"].value = "Some value\n";
doc["element"].user = "Some user text\n";
doc.close().user = "Close user\n";
doc.close().value = "Close value\n";
doc.date().user = "Date user\n";
doc.date().value = "Date value\n";
doc.preamble().user = "preamble user\n";
doc.preamble().value = "preamble value\n";
doc.version().user = "version user\n";
doc.version().value = "version value\n";
doc.postscript() = "postscript\n";

QString buf;
QTextOStream os(&buf);

doc.start_phrase( "## BEGIN " );
doc.end_phrase( "## END " );
doc.write( os );

QString canon{ "version user\n## BEGIN version\nversion value\n## END version\nDate user\n## BEGIN date\nDate value\n## END date\npreamble user\n## BEGIN preamble\npreamble value\n## END preamble\nSome user text\n## BEGIN target element\nSome value\n## END target element\nClose user\n## BEGIN close\nClose value\n## END close\npostscript\n" };

BOOST_CHECK(buf == canon);

}

