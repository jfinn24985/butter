
#include "build_style_test.h"
#include "build_style.h"
#include "text_template.h"


#include <boost/test/unit_test.hpp>
#include <QXmlStreamReader>

static std::ostream & operator<<(std::ostream & os, const QString &s)
{
  os << s.toStdString();
  return os;
}
// Return a canonical XML document usable for testing.
//
// style = gmake
// 2 x files (system.mk, local.mk)
// 4 x properties (root-file, build-file, executable-install-location, executable-install-mode)
// 9 x templates (comment, start-phrase, end-phrase, root-preamble, preamble, package,
//                build-target, install-target, sublocation)
static const QByteArray & xml_doc()
{
  static QByteArray xmldoc{ "<style label='gmake'>\n<file label='system.mk'>## system.mk\n#\n# System Makefile\n#\n</file>\n<file label='local.mk'>## local.mk.tmpl\n\nOS ?= LINUX\nCOMP ?= GNU\nVARIANT ?= DEBUG\n</file>\n<!-- name of the top-most generated buildfile -->\n<property label='root-file'>makefile</property>\n\n<!-- name of build file in any sub-directories -->\n<property label='build-file'>makefile</property>\n<!-- default for install flags -->\n<property label='executable-install-location'>$(BINDIR)</property>\n<property label='executable-install-mode'>$(BINIFLAGS)</property>\n\n<!-- how to make comments -->\n<template label='comment'>## <replace label='text'/></template>\n<template label='start-phrase'>## START:<replace label='name'/>\n</template>\n<template label='end-phrase'>## END:<replace label='name'/>\n</template>\n\n\n<!-- template for start of root makefile -->\n<template label='root-preamble'><option label='rootdir'>export ROOTDIR:=<replace label='location_root'/></option>\ninclude $(ROOTDIR)/system.mk\n</template>\n\n<!-- template for start of subdirectory makefiles -->\n<template label='preamble'>\ninclude $(ROOTDIR)/system.mk\n</template>\n\n<!-- template for start of (internal) package -->\n<template label='package'>\n<option label='SYSINCLUDE'>export SYSINCLUDE:= <replace label='package_includes'/>\n</option><option label='SYSCFLAGS'>export SYSCFLAGS:= <replace label='package_cflags'/>\n</option><option label='SYSC++FLAGS'>export SYSC++FLAGS:= <replace label='package_c++flags'/>\n</option><option label='SYSFFLAGS'>export SYSFFLAGS:= <replace label='package_fflags'/>\n</option><option label='SYSLDFLAGS'>export SYSLDFLAGS:= <replace label='package_ldflags'/>\n</option></template>\n\n<!-- template for a target -->\n<template label='build-target'>\n<option label='SRC'><replace label='TARGET_NAME'/>SRC:=<replace label='target_src'/>\n</option><option label='INCLUDE'><replace label='TARGET_NAME'/>INCLUDE:= $(SYSINCLUDE) <replace label='target_include'/>\n</option><option label='CFLAGS'><replace label='TARGET_NAME'/>CFLAGS:= $(SYSCFLAGS) <replace label='target_cflags'/>\n</option><option label='C++FLAGS'><replace label='TARGET_NAME'/>C++FLAGS:= $(SYSC++FLAGS) <replace label='target_c++flags'/>\n</option><option label='LDFLAGS'><replace label='TARGET_NAME'/>LDFLAGS:= $(SYSLDFLAGS) <replace label='target_ldflags'/>\n</option><option label='DINC'><replace label='TARGET_NAME'/>DINC := $(filter %.$(SUFDEP), $(<replace label='TARGET_NAME'/>SRC:.$(SUFC++SRC)=.$(SUFDEP))) $(filter %.$(SUFDEP), $(<replace label='TARGET_NAME'/>SRC:.$(SUFCSRC)=.$(SUFDEP)))\ninclude $(<replace label='TARGET_NAME'/>DINC)</option>\n\n<replace label='TARGET_NAME'/>OBJ := $(<replace label='TARGET_NAME'/>OBJ) $(filter %.o, $(<replace label='TARGET_NAME'/>SRC:.$(SUFC++SRC)=.$(SUFOBJ)))\nCLEAN_TEMP := $(CLEAN_TEMP) $(<replace label='TARGET_NAME'/>OBJ)\n\n<replace label='target_name'/>$(SUFEXE) :: CFLAGS:=... $(patsubst %,-I%,$(<replace label='TARGET_NAME'/>INCLUDE)) $(VARIANTFLAGS) ...\n<replace label='target_name'/>$(SUFEXE) :: C++FLAGS:=... $(<replace label='TARGET_NAME'/>INCLUDE) ...\n<replace label='target_name'/>$(SUFEXE) :: $(<replace label='TARGET_NAME'/>OBJ) <replace label='target_object'/> <replace label='target_local_libs'/> | $(<replace label='TARGET_NAME'/>DINC) \n#?	$(C++) $(C++FLAGS) -o <replace label='target_name'/>$(SUFEXE) $(<replace label='TARGET_NAME'/>OBJ) $(<replace label='TARGET_NAME'/>LDFLAGS) $(LDFLAGS)\n\nall :: <replace label='target_name'/>$(SUFEXE)\n\nCLEAN_TARGETS := $(CLEAN_TARGETS) <replace label='target_name'/>$(SUFEXE)\n</template>\n\n<template label='executable-install-target'>\ninstall:: <replace label='target-name'/>$(SUFEXE)\n<option label='where'>	$(INSTALL) -d <replace label='executable-install-location'/>\n</option><option label='install'>	$(INSTALL) <replace label='executable-install-mode'/> $&lt; <replace label='executable-install-location'/>/$&lt;\n</option>\n</template>\n<template label='sublocation'>\nDIRS:= $(DIRS) <replace label='location_dir'/></template>\n<template label='close'># Local Variables:\n# mode:makefile\n# End:\n</template><template label='version'># Butter version <replace label='butter-version'/>\n<option label='version'># Project version <replace label='version'/>\n</option></template><template label='date'># DATE: <replace label='date'/>\n</template></style>" };
  return xmldoc;
}


BOOST_AUTO_TEST_CASE( build_style_constructor )
{
// Static Lifetime method tests
BOOST_CHECK( std::is_default_constructible< butter::build_style >::type{} );
BOOST_CHECK( ! std::is_copy_constructible< butter::build_style >::type{} );
BOOST_CHECK( ! std::is_move_constructible< butter::build_style >::type{} );
BOOST_CHECK( ! (std::is_assignable< butter::build_style, butter::build_style >::type{}) );
BOOST_CHECK( ! std::has_virtual_destructor< butter::build_style >::type{});

// test constructor
{
  butter::build_style tmpl{};
  BOOST_CHECK(tmpl.get_build_file_name().isEmpty());
  BOOST_CHECK(tmpl.get_root_file_name().isEmpty());
  BOOST_CHECK(tmpl.get_special_files().empty());
  BOOST_CHECK(tmpl.get_templates().empty());
  BOOST_CHECK(tmpl.get_properties().empty());
}

}

BOOST_AUTO_TEST_CASE( build_style_read_basic )
{
{
  butter::build_style var;
  
  QXmlStreamReader x(xml_doc());
  x.readNextStartElement();
  var.read_style("gmake", x);
  BOOST_CHECK_EQUAL(4, var.get_properties().size());
  BOOST_CHECK_EQUAL(12, var.get_templates().size());
  BOOST_CHECK_EQUAL(2, var.get_special_files().size());
}

}

BOOST_AUTO_TEST_CASE( build_style_read_properties )
{
{
  butter::build_style var;
  
  QXmlStreamReader x(xml_doc());
  x.readNextStartElement();
  var.read_style("gmake", x);
  BOOST_CHECK_EQUAL("makefile", var.get_build_file_name());
  BOOST_CHECK_EQUAL("makefile", var.get_root_file_name());
  {
    butter::build_style::map_type::const_iterator itr = var.get_properties().find("executable-install-location");
    BOOST_REQUIRE( itr != var.get_properties().end() );
    BOOST_CHECK_EQUAL(QString("$(BINDIR)"), itr.value());
  }
  {
    butter::build_style::map_type::const_iterator itr = var.get_properties().find("executable-install-mode");
    BOOST_REQUIRE( itr != var.get_properties().end() );
    BOOST_CHECK_EQUAL(QString("$(BINIFLAGS)"), itr.value());
  }
}

}

BOOST_AUTO_TEST_CASE( build_style_read_special_files )
{
{
  butter::build_style var;
  
  QXmlStreamReader x(xml_doc());
  x.readNextStartElement();
  var.read_style("gmake", x);
  {
    butter::build_style::map_type::const_iterator itr = var.get_special_files().find("system.mk");
    BOOST_REQUIRE( itr != var.get_special_files().end() );
    BOOST_CHECK_EQUAL(QString("## system.mk\n#\n# System Makefile\n#\n"), itr.value());
  }
  {
    butter::build_style::map_type::const_iterator itr = var.get_special_files().find("local.mk");
    BOOST_REQUIRE( itr != var.get_special_files().end() );
    BOOST_CHECK_EQUAL(QString("## local.mk.tmpl\n\nOS ?= LINUX\nCOMP ?= GNU\nVARIANT ?= DEBUG\n"), itr.value());
  }
}

}

BOOST_AUTO_TEST_CASE( build_style_make_x_no_tmpl )
{
// test constructor
{
  butter::build_style tmpl{};
  butter::build_style::map_type p{};
  BOOST_CHECK(tmpl.make_close(p).isEmpty());
  BOOST_CHECK_EQUAL("# X\n", tmpl.make_comment_text("X"));
  BOOST_CHECK_EQUAL("# START:name\n", tmpl.make_start_phrase("name"));
  BOOST_CHECK_EQUAL("# END:name\n", tmpl.make_end_phrase("name"));
  BOOST_CHECK(tmpl.make_element("binary", p).isEmpty());
  BOOST_CHECK(tmpl.make_preamble(p).isEmpty());
  BOOST_CHECK(tmpl.make_version(p).isEmpty());
  p.insert("date", "today");
  BOOST_CHECK_EQUAL("# today\n", tmpl.make_date(p));
}

}

BOOST_AUTO_TEST_CASE( build_style_make_x_some_tmpl )
{
// test make_* methods after reading in xml_doc
{
  butter::build_style tmpl{};
  QXmlStreamReader x(xml_doc());
  x.readNextStartElement();
  tmpl.read_style("gmake", x);
  butter::build_style::map_type p{};
  BOOST_CHECK_EQUAL("# Local Variables:\n# mode:makefile\n# End:\n", tmpl.make_close(p));
  BOOST_CHECK_EQUAL("## X\n", tmpl.make_comment_text("X"));
  BOOST_CHECK_EQUAL("## START:name\n", tmpl.make_start_phrase("name"));
  BOOST_CHECK_EQUAL("## END:name\n", tmpl.make_end_phrase("name"));
  p.insert("target-name", "unittest");
  BOOST_CHECK_EQUAL("\ninstall:: unittest$(SUFEXE)\n\t$(INSTALL) -d $(BINDIR)\n\t$(INSTALL) $(BINIFLAGS) $< $(BINDIR)/$<\n\n", tmpl.make_element("executable-install-target", p));
  BOOST_CHECK_EQUAL("\ninclude $(ROOTDIR)/system.mk\n", tmpl.make_preamble(p));
  p.insert("butter-version", "0.2beta");
  p.insert("version", "0.1");
  BOOST_CHECK_EQUAL("# Butter version 0.2beta\n# Project version 0.1\n", tmpl.make_version(p));
  p.insert("date", "today");
  BOOST_CHECK_EQUAL("# DATE: today\n", tmpl.make_date(p));
}

}

