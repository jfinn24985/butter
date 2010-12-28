

import xml.sax, threading, os.path

_DEBUG = False
if _DEBUG:
  print """<html encoding='UTF-8'><body><style type="text/css">pre.debug{color:red}</style>"""

class option(object):
    """Class to manage application arguments"""
    def __init__(self, a_flag, a_has_param = False, a_default = False):
        #The option value or True/False
        self.value = a_default
        import sys
        for index in range(len(sys.argv)):
           if a_flag == sys.argv[index]:
             # handle pos args
             if a_has_param:
               self.value = sys.argv[index + 1]
             else:
               self.value = True
             # Finished
             return
        pass
        
    
class write_helper(object):
    """This class prints "end_" to "os_" when it is deleted.  It is intended
to help ensure that markup like XML/HTML is well formed by 
creating a object of this type with end_ being a closing tag
when a start tag is generated."""
    def __del__(self):
        """Terminate section of formatted output"""
        import sys
        os_ = self.os_ or sys.stdout
        print >>os_, self.end_
        
    def __init__(self, a_end, a_os):
        """End section of formatted output"""
        #String to print on closing
        self.end_ = a_end
        self.os_ = a_os
        pass
        
    
class object_interface(object):
    #dictionary of HTML document mark_up
    _parts = { # record: ( element, title_element, attrs )
  "document" : ( "<html><body>", "</body></html>", "h1"),
  "job" : ( "div", "h2", {"class": "job"} ),
  "stage" : ( "div", "h3", {"class": "stage"} ),
  "sub-stage" : ( "div", "h4", {"class": "sub-stage"} ),
  "report" : ("div", "h3", {"class":"report"} ),
  "emph" : ("em",),
  "table": ("table", None, {"border":"1"}),
  "tablerow": ("tr",),
  "tablehead": ("th",),
  "tableitem": ("td",),
  "par" : ("p", "em"),
  "divider" : ("hr",),
  "debug" : ( "pre", "em", {"class":"debug"}),
  "error" : ( "div", "strong", {"class":"error"}),
  "code" : ( "pre", "h4" ),
  "output" : ( "pre", "h4")
}
    #Variable used for pretty printing with __str__
    __depth = 0
    def add_characters(self, text):
        """add text to this object iff text is not whitespace

"""
        if not text.isspace ():
           self.content += text
        
    def add_child(self, child):
        """Add an object_interface child object to this object

/pre isinstance(child, object_interface)"""
        assert isinstance(child, object_interface)
        if not self._children:
          self._children = []
        self._children.append (child)
        child._parent = self
        
    def end_load(self):
        """function called when object's content has been
loaded.  In SAX document parsing call this in
response to the end-element event."""
        pass
        
    def __getitem__(self, name):
        """Attempt to find value for given property."""
        # If not found return None
        return None
        
        
    def parent(self):
        """Get this object's parent (should be None for test_suite objects)"""
        return self._parent
        
    def __str__(self):
        """Convert object to a string"""
        Result = "Depth[" + str(object_interface.__depth) + "]" + str(type(self))
        Result = Result + "\n content ["
        Result = Result + self.content
        Result = Result + "]\n"
        if self._children:
          counter = 1
          object_interface.__depth += 1
          for child in self._children:
            Result = Result + "Child [" + str(object_interface.__depth - 1) + "][" +  str(counter) + "]\n"
            Result = Result + str(child)
            counter += 1
          object_interface.__depth -= 1
        return Result
        
    @staticmethod
    def start(elem, args = None, a_os = None):
        """Print openning of element and return object that closes element"""
        import sys
        os_ = a_os or sys.stdout
        if not elem in object_interface._parts:
          print >>sys.stderr, "Invalid output element id: " + elem
          raise KeyError("Invalid output element id: " + elem)
        part_ = object_interface._parts[elem]
        args_ = args or {}
        title_ = None
        anchor_ = None
        if 3 <= len(part_):
          args_.update(part_[2])
        # Do print
        print >>os_, "<" + part_[0],
        may_have_title_ = 2 <= len(part_) and part_[1]
        for key_ in args_:
          if may_have_title_ and key_ in ( "#title", "#target", "#href" ):
            if "#title" == key_:
              title_ = ( "<" + part_[1] + ">", args_[key_], "</" + part_[1] + ">" )
            elif "#target" == key_:
              anchor_ = ( "<a name=\"" + args_[key_] + "\">", "</a>" )
            else:
              anchor_ = ( "<a href=\"#" + args_[key_] + "\">", "</a>" )
          else:
            print >>os_, " " + key_ + "=\"" + args_[key_] + "\"",
        print >>os_, ">",
        if title_:
          if anchor_:
            print >>os_, title_[0], anchor_[0], title_[1], anchor_[1], title_[2]
          else:
            print >>os_, title_[0], title_[1], title_[2]
        return write_helper ("</" + part_[0] + ">", a_os)
        
    def __init__(self):
        #The list of children
        self._children = None
        #Unicode string of content
        self.content = u""
        #The (object_interface) parent object of this object
        self._parent = None
        pass
    
class interpreter(xml.sax.handler.ContentHandler, object):
    """The handler for a SAX parser that converts job description XML into
job objects. """
    #A map of XML element names to object contructors. The
    #constructors can be a single function or a list of
    #functions.
    __constructor_set = None
    @staticmethod
    def add_constructor(name, fntor):
        """Add constructor to the set."""
        if not interpreter.__constructor_set:
          interpreter.__constructor_set = {}
        if not name in interpreter.__constructor_set:
          interpreter.__constructor_set[name] = []
        interpreter.__constructor_set[name].append (fntor)
        if _DEBUG:
          tmp_ = object_interface.start("debug")
          print "Added: ", name, " : ", fntor
          del tmp_
        pass
        
    def __init__(self, top):
        """Create handler for deserialising objects."""
        #The current object.
        self.cursor = None
        #The depth in the XML doc tree
        self.depth = 0
        #The current location in the document
        self.location = None
        #The initial object.
        self.top = top
        assert isinstance(self.top, object_interface)
        
    def parse(self, fname):
        """Parse XML document fname"""
        # Set up parser
        _parser = xml.sax.make_parser ()
        _parser.setContentHandler (self)
        # Start parsing
        _parser.parse (fname)
        
    def characters(self, content):
        """(private) Handle text"""
        self.cursor.add_characters (content)
        
    def endDocument(self):
        """Notification of end of document"""
        self.valid = False
        
    def endElement(self, name):
        """Notification of end of element"""
        self.depth -= 1
        self.cursor.end_load ()
        if self.cursor != self.top:
          self.cursor = self.cursor.parent ()
        
    def endElementNS(self, name, qname):
        """aliased to endElement()"""
        self.endElement(name[1])
        
    def setDocumentLocator(self, locator):
        """Set a location variable that can be used to highlight error point in the XML
document"""
        self.location = locator
        
    def startDocument(self):
        """Notify that document has started.  Initialise some variables-"""
        self.valid = (self.top != None)
        assert self.valid
        self.cursor = self.top
        
    def startElement(self, name, attr):
        _obj = None
        if _DEBUG:
          tmp_ = object_interface.start("debug")
          print "Number of makers: ", len(interpreter.__constructor_set)
          print interpreter.__constructor_set.keys()
          del tmp_
        assert name in interpreter.__constructor_set, "The XML element <" + name + " ...> is not recognised\n" + self._location_to_string ()
        for __ctor in interpreter.__constructor_set[name]:
           _obj = __ctor(name, attr)
           if None != _obj:
              self.cursor.add_child (_obj)
              self.cursor = _obj
              break
        if None == _obj:
          raise IndexError("No match for XML element <" + name + " " + str([str(v) + "=" + str(attr[v]) for v in attr.keys()]) + ">\n" + self._location_to_string ())
        
        self.depth += 1
        
    def startElementNS(self, name, qname, attr):
        self.startElement(name[1],attr)
        
    def _location_to_string(self):
        """Convert self.location to a string"""
        fname_ = self.location.getSystemId()
        if not fname_:
          fname_ = self.location.getPublicId()
          if not fname_:
            fname_ = "buffer"
        return """at %s: line %d, column %d""" % ( fname_, self.location.getLineNumber(), self.location.getColumnNumber() )
        
    
class subfile_interpreter(interpreter):
    """Version of "interpreter" class to handle sub-files"""
    def __init__(self, top):
        """Create handler for deserialising objects."""
        super(subfile_interpreter, self).__init__(top)
        
    def startElement(self, name, attr):
        # If this is the document element return
        if name in ( job.element_name, test_suite.element_name ):
          if not isinstance(self.cursor, (job, test_suite)):
            raise RuntimeError("<job> or <test_suite> element encountered in subfile that was not the document element.")
          return
        # if this is any other element act normal.
        super(subfile_interpreter, self).startElement (name, attr)
        
    def endElement(self, name):
        # If this is the document element return
        if name in ( job.element_name , test_suite.element_name ):
          return
        # if this is any other element act normal.
        super(subfile_interpreter, self).endElement (name)
        
    
class constructor(object):
    """Dummy class that's role is to add constructors to the intepreters
constructor set."""
    def __init__(self, name, fntor):
        """Construct object with the given callable for the given XML element

/param name  XML element name

/param fntor  Functor to create object for XML element.  Signature is
              fntor(name: string; attributes: dictionary): object_interface"""
        interpreter.add_constructor (name, fntor)
        
    
class property(object_interface):
    """Meta-object class for name:value pair tuples in XML that will be converted 
to a python dictionary.

<property @name>value</>"""
    __ctor = constructor(u"property", lambda name, attr: property.make(name, attr))
    #The XML element name for this type
    element_name = u"property"
    #The XML attribute name for this type's 
    #"name" attribute
    name_attr = u"name"
    #The XML attribute name for this type's 
    #"if" attribute
    if_attr = u"if"
    def __init__(self, a_name):
        """Create an instance of property with the given name"""
        super(property, self).__init__()
        #The property's name
        self.name = a_name
        self.if_property = None
        self.if_value = None
        pass
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == property.element_name
        assert elemattr.has_key(property.name_attr)
        Result = property(elemattr[property.name_attr])
        if  elemattr.has_key(property.if_attr):
          ( Result.if_property, Result.if_value ) = elemattr[property.if_attr].split ("=", 2)
        return Result
        
    def conditional(self, a_dict):
        """checks if this is a conditional property and if the
condition is met from the given dictionary."""
        if self.if_property: # Check for dependencies
        need_match = True # Do we match or not match value?
        chk = self.if_value
        if chk[0] == '!':
          chk = chk[1:]
          need_match = False
        value = a_dict.get (self.if_property, None)
        if value:
          return (value == chk) == need_match
        else:
          return not need_match # Need match to add
        return True
        
    
class property_holder(object_interface):
    def add_action(self, action):
        """Append action string to current action"""
        self._action += action
        
    def add_child(self, child):
        """Add a sub object to this object"""
        if isinstance(child, property):
          if None == self._property_list:
            self._property_list = []
          self._property_list.append(child)
          child._parent = self
        else:
          # Call parent class method
          super(property_holder,self).add_child(child)
        assert None != child._parent
        
    def add_test(self, a_phrase):
        """Add an acceptance test phrase, return an
integer that identifies this phrase"""
        return self.parent ().add_test (a_phrase)
        
    @staticmethod
    def __chkpath(matchobj):
        """Internal method to normalise any file paths in
"PATH{..}" in input"""
        if matchobj.group(1):
          return os.path.normpath(matchobj.group(1))
        else:
          return ""
        pass
        
    def fail_test(self, a_id, a_is_nonfatal):
        """Indicate test with a_id has failed."""
        self.parent ().fail_test (a_id, a_is_nonfatal)
        
    def get(self, key, default_val):
        Result = self.__getitem__(key)
        if not Result:
          return default_val
        return Result
        
    def get_action(self):
        """Get action string"""
        return self._action
        
    def __getitem__(self, name):
        """Attempt to find value for given property.

Programming Note: _replace must be checked first
to allow manually set values to override values set
via properties."""
        # Check in replacements first
        if self._replace:
          if self._replace.has_key (name):
            return self._replace[name]
        # If find a value return it
        if self._property_list:
          for _nv in self._property_list:
            if name == _nv.name:
              if _nv.conditional (self):
                return _nv.content
        # not in this object, try parent
        if None != self.parent ():
          return self.parent ().__getitem__ (name)
        # If no parent return None
        return None
        
        
    import re
    __format_re = re.compile("(%\([^(]*?\)s)+")
    __path_re = re.compile("PATH\{([^}]*)\}")
    __envvar_re = re.compile("ENVVAR\{([^}]*)\}")
    def make_substitutions(self, phrase):
        """Replace all %(...)s in phrase using this object as
the dictionary, repeat until no further changes occur.
Return result, leave phrase unaltered."""
        tmp1_ = phrase
        while self.__format_re.search (tmp1_):
          tmp1_ = tmp1_ % self
        import os
        if os.name == "nt":
          return self.__path_re.sub(self.__chkpath, self.__envvar_re.sub(r"%\1%", tmp1_))
        else:
          return self.__path_re.sub(self.__chkpath, self.__envvar_re.sub(r"${\1}", tmp1_))
        pass
        
    def pass_test(self, a_id, a_is_nonfatal):
        """Indicate test with a_id has passed."""
        self.parent ().pass_test (a_id, a_is_nonfatal)
        
    def reset_action(self):
        """Reset action string"""
        self._action = ""
    def __str__(self):
        """Add property list to string"""
        Result = super(property_holder, self).__str__()
        if self._property_list:
          Property = " property list:"
          for prop_ in self._property_list:
            Property = Property + "\n   " + prop_.name + " = " + prop_.content
          delim = "\n"
          ( start, end ) = Result.split(delim, 1)
          return delim.join( ( start, Property, end) )
        else:
          return Result
        
    def substitutions(self, key, value):
        """Update the replacement dictionary

if key != None:
  add_to_dictionary (key, value)
else: # assume value is {...}
  set_dictionary_to (value)"""
        if key != None:
          if not self._replace:
            self._replace = {}
          self._replace[key] = value
        else: # assume value is {}
          assert isinstance(value, type({})), "Expected a dictionary object"
          self._replace = value
        pass
        
    def __init__(self):
        super(property_holder, self).__init__()
        #The set of python commands to perform.
        self._action = ""
        #Lock to serialise conversion of property dictionary if this object can be accessed
        #from several threads (Default is no-threads)
        self._dict_lock = None
        #The unconverted list of property objects
        self._property_list = None
        #Replacements dictionary for string substitution
        self._replace = None
        pass
    
class subfile_holder(property_holder):
    """Derived class of property holder that can contain "src" attribute
to allow inclusion of sub-files.  Derived classes load the sub-file
by calling "load-subfile"."""
    src_name = u"src"
    def load_subfile(self):
        """If this element contained @src then read in the document."""
        # Build python evaluation string
        if self._src:
          if _DEBUG:
            tmp_ = self.start("debug")
            print "Have sub-file \"" + self._src + "\"; reading file."
            del tmp_
          parser_ = subfile_interpreter (self)
          parser_.parse (self._src)
          del parser_
        pass
        
    def set_src(self, elemattr):
        """Search attributes for "src" attr."""
        self._src = elemattr.get (self.src_name, None)
        
    def __init__(self):
        super(subfile_holder, self).__init__()
        #Option attribute value for a sub-file containing
        #more elements to place in this object.  None indicates
        #that no sub-file has been set.
        self._src = None
        pass
    
class verb(property_holder):
    """An "atomic" action performed during a test.  It can be in
one of four categories: setup, execute, verify and cleanup

<verb @type>
  <property @name>value</>
  ...
</>

See individual types for expected name:value properties"""
    element_name = u"verb"
    #What to do with certain errors
    _error = { u"fatal": """raise RuntimeError(r"%(_message_)s")"""
 , u"nonfatal": """self.passed = False; print r"%(_message_)s" """
 , u"optional" : """print r"%(_message_)s" """  }
    test_property_name = u"test-phrase"
    #The XML attribute name for the verb's 
    #"type" attribute
    type_attr = u"type"
    def end_load(self):
        """Base-class version should be called at end of derived class methods. It
will handle "test" property."""
        self.do_end_load ()
        # The action should have been built in derived classes
        assert self.get_action (), "Action needs to be set in derived classes"
        # If search for "test" property
        if self._property_list:
          for _nv in self._property_list:
            if verb.test_property_name == _nv.name:
              # This verb performs a specific test
              test_id_ = self.add_test (self.make_substitutions (_nv.content))
              action_ = self.get_action ()
              self.reset_action ()
              #Indent lines 2 spaces
              import string
              action_ = string.join(map(lambda a: "  " + a, action_.split("\n")),"\n")
              self.add_action ("try:\n" + action_ + "\n  self.pass_test (" + str(test_id_) +
               ", self.passed)\nexcept:\n  self.fail_test (" + str(test_id_) + ", self.passed)\n  raise\nself.passed = True")
        pass
        
    def __init__(self):
        super(verb, self).__init__()
        pass
    
class verb_checkout(verb):
    """Gets something from a version control repository or archive

Directly supported types are svn, cvs, tar and zip.  However,
the program that performs the operation can be specified
using a property which allows arbitrary systems to be used. 

Properties:
url          : base url/path of archives
branch  : branch/archive name
base     : target base directory
type      : Type of archive or repository (svn,cvs,tar,zip)
svn-command : program to handle subversion check-outs ("svn")
cvs-command : program to handle cvs check-outs ("cvs")
For "tar" type:
tar-command : program to handle tar files
compress : tar's option to indicate compression type (default "")
For "zip" type:
unzip-command  : program to decompress archive (default="unzip")
to-dir         : flag and location of base output directory (default="-d %(base)")

The templates for the various operations are:
%(svn-command) co %(location) %(base)
%(cvs-command) -d %(url) co %(branch) -d %(base)
%(tar-command) %(compress) -x -f %(location) -D %(base)
%(unzip-command) %(location) %(to-dir)"""
    __ctor = constructor(verb.element_name, lambda name, attr: verb_checkout.make(name, attr))
    #How to handle different file/dir types
    _checkout = { u"svn": ( """%(_cmd_name_)s co %(location)s %(base)s""", """Unable to checkout branch from SVN repository %(location)s""" )
, u"cvs": ( """%(_cmd_name_)s -d %(url)s co %(branch)s -d %(base)s""", """Unable to checkout from CVS branch %(branch)s from repository %(url)s""" )
,  u"tar": ( """%(_cmd_name_)s %(compress)s -x -f %(location)s -D %(base)s""", """Unable to unpack archive %(location)s""" )
,  u"zip": ( """%(_cmd_name_)s %(location)s %(_to_dir_)s""", """Unable to unpack archive %(location)s""" )
}
    #The XML attribute value for this verb's 
    #"type" attribute
    type_value = u"check-out"
    def do_end_load(self):
        """Compile the verb's actions"""
        # Build python evaluation string
        type_ = self.make_substitutions(self.get (u"type", u"svn"))
        ( cmd_, msg_ ) = verb_checkout._checkout[type_]
        phrase_ = u"""self.system_call (r\"\"\"%(_command_)s\"\"\", r\"\"\"%(_error_)s\"\"\")\n"""
        todir_ = self.get (u"to-dir", u"-d %(base)s")
        cmd_name_ = self.get (type_ + u"-command", type_)
        self.substitutions(None, { u"_command_": cmd_, u"_message_": msg_, u"_to_dir_": todir_, u"_error_":  verb._error[self.get (u"error",u"fatal")], u"_cmd_name_": cmd_name_ } )
        self.reset_action ()
        self.add_action (self.make_substitutions(phrase_))
        
        if _DEBUG:
          tmp_ = self.start("debug", {"#title":"Verb checkout data:"})
          print self.get_action ()
          del tmp_
        pass
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == verb.element_name
        assert elemattr.has_key (verb.type_attr)
        if elemattr[verb.type_attr] == verb_checkout.type_value:
          return verb_checkout()
        else:
          return None
        
    def __init__(self):
        super(verb_checkout, self).__init__()
        pass
    
class verb_compare(verb):
    """Perform the most rigourous comparison using some program to compare 
the contents of a file with some canonical example, the default is to
use "diff". You can specifiy program options, for example using the GNU
diff option to exclude comparing lines that will always be different such as 
datestamps and timestamps.  Of course other programs can be used to
perform other types of tests, eg CRC, file mode etc.

Command phrase:
%(compare-command) %(compare-args) %(base)/%(leaf) %(compare-base)%(leaf)%(suffix)
or
%(search-command) %(search-args) %(search-phrase) %(base)/%(leaf)
where leaf is a single element in "location"

Properties
type : compare | search
compare-command  : The comparison program (default = "diff")
compare-args : Arguments to program (default = "-q")
search-command  : The search program (default = "grep")
search-args : Arguments to program (default = "")
search-regex : Regular expression to search for (required)
suffix                 : File suffix to add to file(default = ".canon")
base                 : Base directory
compare-base : Directory with canonical files (defaut = base)
location            : List of paths for comparison."""
    __ctor = constructor(verb.element_name, lambda name, attr: verb_compare.make(name, attr))
    #The XML attribute value for this verb's 
    #"type" attribute
    type_value = u"compare"
    def do_end_load(self):
        """function called when object's content has been fully
loaded.  In SAX document parsing call this in response
to the end-element event."""
        """
        The output of this function:
        
        * generate list of locations
        
        * if file-type is "file" then we
        only check for existence
        """
        # Build replacement dictionary and phrase for this type
        type_ = self.get (u"type", u"compare")
        base1_ = self.make_substitutions(self[u"base"])
        base2_ = None
        msg_ = None
        phrase_ = None
        # Settings for "diff"
        if type_ == u"compare":
          compare_ = self.get (u"compare-command", u"diff")
          args_ = self.get (u"compare-args", u"")
          suffix_ = self.get (u"suffix", u".canon")
          msg_ = u"""File %(_path1_)s does not compare equal to canonical file %(_path2_)s%(_suffix_)s"""
          phrase_ = u"""self.system_call(r\"\"\"%(_compare_)s %(_compare_args_)s %(_path1_)s %(_path2_)s%(_suffix_)s\"\"\", r\"\"\"%(_error_)s\"\"\")\n"""
          base2_ = self.make_substitutions(self.get(u"compare-base", u""))
          if not base2_:
            base2_ = base1_
          # Initialise the object-specific substitution dictionary
          self.substitutions( None, { u"_compare_": compare_, u"_compare_args_": args_, u"_path1_": None, u"_path2_": None, u"_message_": None,  u"_error_": verb._error[self.get (u"error",u"fatal")], u"_suffix_": suffix_ } )
        else: # assume grep!
          search_ = self.get (u"search-command", u"grep")
          args_ = self.get (u"search-args", u"")
          regex_ = self.get (u"search-regex", None)
          if not regex_:
            raise RuntimeError("""No "search-regex" set for search""")
          msg_ = u"""No lines in file %(_path1_)s match regular expression %(_regex_)s"""
          phrase_ = u"""self.system_call(r\"\"\"%(_search_)s %(_search_args_)s %(_regex_)s %(_path1_)s\"\"\", r\"\"\"%(_error_)s\"\"\")\n"""
          # Initialise the object-specific substitution dictionary
          self.substitutions( None, { u"_search_": search_, u"_search_args_": args_, u"_path1_": None, u"_regex_": regex_, u"_message_": None,  u"_error_": verb._error[self.get (u"error",u"fatal")] } )
          phrase_ = u"""self.system_call(r\"\"\"%(_search_)s %(_search_args_)s "%(_regex_)s" %(_path1_)s\"\"\", r\"\"\"%(_error_)s\"\"\")\n"""
        
        # Build python evaluation string
        self.reset_action ()
        
        for leaf_ in self[u"location"].split(u" "):
          # update dictionary for current file
          self.substitutions(u"_path1_", os.path.normpath(os.path.join(base1_, leaf_)))
          if base2_:
            self.substitutions(u"_path2_", os.path.normpath(os.path.join(base2_, leaf_)))
          self.substitutions(u"_message_", self.make_substitutions(msg_))
          # append actions
          self.add_action (self.make_substitutions(phrase_))
        
        if _DEBUG:
          tmp_ = self.start("debug", {"#title":"Verb compare data:"})
          print self.get_action ()
          del tmp_
        pass
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == verb.element_name
        assert elemattr.has_key(verb.type_attr)
        if elemattr[verb.type_attr] == verb_compare.type_value:
          return verb_compare()
        else:
          return None
        
    def __init__(self):
        super(verb_compare, self).__init__()
        pass
    
class verb_filelist(verb):
    """Perform one of four actions on a set of files or directories. The
actions are:
exist       : test if path exists
nonexist : test if path does not exist
recreate : remove any existing path and recreate path
remove  : remove a path

standard properties:
severity  : (fatal,nonfatal,optional)
file-type : (directory,file,executable*)
verify      : action to perform or test (remove,exist,recreate,nonexist)
location : list of paths to check
base      : base directory for locations

*currently an executable is treated the same as a file (ie no 
permission checks)"""
    __ctor = constructor(verb.element_name, lambda name, attr: verb_filelist.make(name, attr))
    #How to handle different file/dir types
    _filetype = { u"file": ( """(os.path.isfile(r"%(_path_)s") or verb_filelist.findfile(r"%(_path_)s"))""", """os.unlink(r"%(_path_)s")""", """os.close(os.open(r"%(_path_)s"))""", r"""Problem with file (action: %(_action_)s): %(_path_)s""" )
, u"executable": ( """(os.path.isfile(r"%(_path_)s") or verb_filelist.findfile(r"%(_path_)s"))""", """os.unlink(r"%(_path_)s")""", """os.close(os.open(r"%(_path_)s"))""", r"""Problem with file (action: %(_action_)s): %(_path_)s""" )
,  u"directory": ( """os.path.isdir(r"%(_path_)s")""", """verb_filelist.removedirs(r"%(_path_)s")""", """os.makedirs(r"%(_path_)s")""", r"""Problem with directory (action: %(_action_)s): %(_path_)s""" )
}

    #How to perform different test conditions
    _verify = { u"exist": """if not (os.path.exists(r"%(_path_)s") or verb_filelist.findfile(r"%(_path_)s")):\n  %(_error_)s\n"""
, u"nonexist": """if (os.path.exists(r"%(_path_)s") or verb_filelist.findfile(r"%(_path_)s")):\n  %(_error_)s\n"""
, u"create": """if not os.path.exists(r"%(_path_)s"):\n  %(_create_)s\n"""
, u"recreate": """if os.path.exists(r"%(_path_)s"):\n  %(_remove_)s\n%(_create_)s\n"""
, u"remove": """%(_remove_)s\n"""
}

    #The XML attribute value for this verb's 
    #"type" attribute
    type_value = u"file-list"
    def do_end_load(self):
        """function called when object's content has been fully
loaded.  In SAX document parsing call this in response
to the end-element event."""
        """
        The output of this function:
        
        * generate list of locations
        
        * if file-type is "file" then we
        only check for existence
        """
        # Build replacement dictionary and phrase for this type
        action_ = self.get (u"verify", u"exist")
        verify_ = verb_filelist._verify[action_]
        if action_ != u"nonexist":
          verify_ += """if """ + (action_ != u"remove" and "not " or "") + """%(_type_)s:\n  %(_error_)s\n"""
        ( type_, remove_, create_, msg_ ) = verb_filelist._filetype[self.get (u"file-type",u"file")]
        error_ = verb._error[self.get (u"error",u"fatal")]
        
        # Initialise the object-specific substitution dictionary
        self.substitutions( None, { u"_action_": action_, u"_remove_": remove_, u"_create_": create_, u"_error_" : error_, u"_type_": type_, u"_path_": None, u"_message_": None } )
        
        base_path_ = self.make_substitutions(self[u"base"])
        
        #
        # Build python evaluation string
        #
        
        self.reset_action ()
        
        for leaf_ in self[u"location"].split(u" "):
          # update dictionary for current file
          self.substitutions(u"_path_", os.path.normpath(os.path.join(base_path_, leaf_)))
          self.substitutions(u"_message_", self.make_substitutions(msg_))
          # append actions
          self.add_action (self.make_substitutions(verify_))
        
        if _DEBUG:
          tmp_ = self.start("debug", {"#title":"Verb filelist data:"})
          print self.get_action ()
          del tmp_
        pass
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == verb.element_name
        assert elemattr.has_key (verb.type_attr)
        if elemattr[verb.type_attr] == verb_filelist.type_value:
          return verb_filelist()
        else:
          return None
        
    @staticmethod
    def findfile(a_path):
        """Test if file exists in a leaf directory."""
        dir = os.path.dirname(a_path)
        leaf = os.path.basename(a_path)
        if not os.path.exists(dir):
          return False
        """Look for subdirs"""
        for root_, dirs_, files_ in os.walk(dir, False):
          if 0 == len(dirs_) and leaf in files_:
            return True
        return False
        
    @staticmethod
    def removedirs(a_path):
        """Remove a path"""
        if os.path.exists(a_path):
          for root_, dirs_, files_ in os.walk(a_path, False):
            for f_ in files_:
                os.unlink(os.path.join(root_, f_))
            for d_ in dirs_:
                os.rmdir(os.path.join(root_, d_))
          os.rmdir(a_path)
        pass
        
    def __init__(self):
        super(verb_filelist, self).__init__()
        pass
    
class verb_list(verb):
    """Meta-object class for categorising the verb sets in the XML file.

<verb-list @category>(<property>..)*(<verb>...</>)*</>"""
    element_name = u"verb-list"
    #The XML attribute name for the verb-list's 
    #"category" attribute
    category_attr = u"category"
    __ctor = constructor(u"verb-list", lambda name, attr: verb_list.make(name, attr))
    def do_end_load(self):
        """Compile all sub-verb actions into a single action.  Delete
children at end."""
        # Run each verb list with quit on fail
        if self._children:
          for item_ in self._children:
            if isinstance(item_, verb):
              self.add_action(item_.get_action ())
              self.add_action(u"\n")
          self._children = None
        # super(verb_list, self).end_load ()
        if _DEBUG:
          tmp_ = self.start("debug", {"#title":"List of verbs data"})
          print self.get_action ()
          del tmp_
        pass
        
    def __init__(self, a_cat):
        self.category = a_cat
        super(verb_list, self).__init__()
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == verb_list.element_name
        assert elemattr.has_key(verb_list.category_attr)
        return verb_list(elemattr[verb_list.category_attr])
        
    
class verb_patch(verb):
    """Apply a patch (or similar) to something.

command is
%(patch-command) -d %(base) %(patch-args) %(patch-dir)/%(leaf)%(suffix)

where leaf is a single element of location property

Properties
patch-command : The comparison program (default = "patch")
patch-args : Arguments to program (default = "-q")
suffix           : File suffix to add to file(default = ".patch")
base           : Base directory
patch-base : Directory with canonical files (defaut = base)
location       : List of patchs to apply"""
    __ctor = constructor(verb.element_name, lambda name, attr: verb_patch.make(name, attr))
    #The XML attribute value for this verb's 
    #"type" attribute
    type_value = u"patch"
    def do_end_load(self):
        """function called when object's content has been fully
loaded.  In SAX document parsing call this in response
to the end-element event."""
        # Build replacement dictionary and phrase for this type
        patch_ = self.get (u"patch-command", u"patch")
        args_ = self.get (u"patch-args", u"-t -i")
        suffix_ = self.get (u"suffix", u".patch")
        msg_ = u"""File %(_path_)s failed to apply"""
        base_ = self.make_substitutions(self[u"base"])
        patch_dir_ = self.make_substitutions(self.get(u"patch-dir", u""))
        if not patch_dir_:
          patch_dir_ = base_
        
        # Initialise the object-specific substitution dictionary
        self.substitutions( None, { u"_patch_": patch_, u"_patch_args_": args_, u"_path_": None, u"_message_": None,  u"_error_": verb._error[self.get (u"error",u"fatal")]
        , u"_base_": base_ } )
        
        phrase_ = u"""self.system_call(r\"\"\"%(_patch_)s -d %(_base_)s %(_patch_args_)s %(_path_)s\"\"\", r\"\"\"%(_error_)s\"\"\")\n"""
        
        # Build python evaluation string
        self.reset_action ()
        
        for leaf_ in self.make_substitutions(self[u"location"]).split(u" "):
          # update dictionary for current file
          self.substitutions(u"_path_", os.path.normpath(os.path.join(patch_dir_, leaf_)))
          self.substitutions(u"_message_", self.make_substitutions(msg_))
          # append actions
          self.add_action (self.make_substitutions(phrase_))
        
        if _DEBUG:
          tmp_ = self.start("debug", {"#title":"Verb patch data:"})
          print self.get_action ()
          del tmp_
        pass
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == verb.element_name
        assert elemattr.has_key (verb.type_attr)
        if elemattr[verb.type_attr] == verb_patch.type_value:
          return verb_patch()
        else:
          return None
        
    def __init__(self):
        super(verb_patch, self).__init__()
        pass
    
class verb_run(verb):
    """Run a piece of inline python code that is the content of this
element.  It may use "%(property-name)s" replacements to 
access properties of the job."""
    __ctor = constructor(verb.element_name, lambda name, attr: verb_run.make (name, attr))
    #The XML attribute value for this verb's 
    #"type" attribute
    type_value = u"run"
    def do_end_load(self):
        """function called when object's content has been fully
loaded.  In SAX document parsing call this in response
to the end-element event."""
        # Build python evaluation string
        self.reset_action ()
        
        self.add_action (self.make_substitutions(self.content))
        
        if _DEBUG:
          tmp_ = self.start("debug", {"#title": "Verb run data:"})
          print self.get_action ()
          del tmp_
        pass
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == verb.element_name
        assert elemattr.has_key (verb.type_attr)
        if elemattr[verb.type_attr] == verb_run.type_value:
          return verb_run()
        else:
          return None
        
    def __init__(self):
        super(verb_run, self).__init__()
        pass
    
class verb_system(verb):
    """Run an arbitrary command using an OS "system" call.  The 
command to execute is given as the XML element's content
and may include "$(property-name)s" replacements to access
job properties."""
    __ctor = constructor(verb.element_name, lambda name, attr: verb_system.make(name, attr))
    #The XML attribute value for this verb's 
    #"type" attribute
    type_value = u"system"
    def do_end_load(self):
        """function called when object's content has been fully
loaded.  In SAX document parsing call this in response
to the end-element event."""
        # Build python evaluation string
        self.reset_action ()
        command_ = """self.system_call(r\"\"\"""" + self.content + """\"\"\", r\"\"\"%(_error_)s\"\"\")\n"""
        self.substitutions(u"_message_", """System command [""" + self.content + """] returned non-zero status""")
        self.substitutions(u"_error_", verb._error[self.get (u"error",u"fatal")])
        self.add_action (self.make_substitutions(command_))
        
        if _DEBUG:
          tmp_ = self.start("debug", {"#title": "Verb system data:"})
          print self.get_action ()
          del tmp_
        pass
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == verb.element_name
        assert elemattr.has_key (verb.type_attr)
        if elemattr[verb.type_attr] == verb_system.type_value:
          return verb_system()
        else:
          return None
        
    def __init__(self):
        super(verb_system, self).__init__()
        pass
    
class job(subfile_holder):
    """A set of instructions that perform a test of a system. These instructions are 
grouped into stages that are executed in series.

* common job specifications can be saved in a separate
 file and loaded using @src.  This file is loaded at the end
 of the element scope allowing properties defined in this
 element to be used in the sub-file. (cf test_suite where the
 sub-file properties are loaded at the start)

<job @src>
  Job description
  <property @name>value<>
  ...
  <stage>
     Stage description
     <verb-list @category>
       <verb @type>..</>
    </>
  </>
  ...
</>

also
<job src="..">...</>
This form has the following consequences:
* The content of the element is read and interpreted as normal.
* When the end element is reached the file indicated by @src is
opened and the top-level <job> tag is treated as if it was the 
current element and it's content appended to the currrent object."""
    __ctor = constructor(u"job", lambda name, attr: job.make(name, attr))
    element_name = u"job"
    test_result = ["NODO", "PASS", "FAIL","DIFF"]
    test_color = ["YELLOW", "GREEN", "RED","BLUE"]
    def add_test(self, a_phrase):
        """Add an acceptance test phrase, return an
integer that identifies this phrase"""
        self._test_list.append([0, a_phrase])
        return len(self._test_list) - 1
        
    def end_load(self):
        self.load_subfile()
        
    def fail_test(self, a_id, a_is_nonfatal):
        """Indicate test with a_id has failed."""
        self._test_list[a_id][0] = 2
        self.fatal_error += 1
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == job.element_name
        result = job ()
        result.set_src (elemattr)
        return result
        
    def _mapfn(self, a_os, a_jobnum, a_jobid, a_test):
        """Write job's report to a_os."""
        tr_ = self.start("tablerow", None, a_os)
        td_ = self.start("tableitem", {"bgcolor": job.test_color[a_test[0]]}, a_os)
        print >>a_os, job.test_result[a_test[0]],
        del td_
        td_ = self.start("tableitem", None, a_os)
        print >>a_os, a_jobnum,
        del td_
        td_ = self.start("tableitem", None, a_os)
        print >>a_os, a_jobid,
        del td_
        td_ = self.start("tableitem", None, a_os)
        print >>a_os, a_test[1],
        del td_
        del tr_
        
    def pass_test(self, a_id, a_is_nonfatal):
        """Indicate test with a_id has passed."""
        if a_is_nonfatal:
          self._test_list[a_id][0] = 1
        else:
          self._test_list[a_id][0] = 3
          self.non_fatal_error += 1
        
    def report(self, a_os, a_jobnum):
        """Write job's report to a_os."""
        jobnum_ = str(a_jobnum)
        jobid_ = str(self.make_substitutions(self.get(u"job-id",u"-")))
        self.start("divider", None , a_os)
        tmp_ = self.start("report",{"#title":"Job " + str(a_jobnum) + ": " + (self.content or "Report"), "#href": "job_" + str(a_jobnum)}, a_os)
        tbl_ = self.start("table",None, a_os)
        map (lambda _test: self._mapfn(a_os, jobnum_,jobid_, _test), self._test_list)
        del tbl_
        del tmp_
        pass
        
    def run(self, a_jobnum):
        """Run the various stages.

May never raise and error"""
        if self._children:
          try:
            import sys
            import cStringIO
            SAVEOUT = sys.stdout
            capture_ = cStringIO.StringIO()
            sys.stdout = capture_
            jobelem_ = self.start("job", {"#title": "Job " + str(a_jobnum) + ": " + (self.content or " execution"), "#target": "job_" + str(a_jobnum)}, capture_)
        
            self.__run (capture_)
        
            self.report (capture_, a_jobnum)
            del jobelem_
            jobelem_ = None
            content_ = capture_.getvalue ()
            capture_.close ()
            sys.stdout = SAVEOUT
            print content_
          except Exception, err:
            tmp_ = self.start("error",{"#title":"Fatal internal error occured"}, sys.stderr)
            print >>sys.stderr, str(err)
            del tmp_
          except:
            jobelem_ = self.start("error", {"#title": "Job "+ str(a_jobnum) + " execution"}, sys.stderr)
            print >>sys.stderr, "test-suite job initialisation failed."
            del jobelem_
        pass
        
    def __run(self, a_os):
        """Internal implementation of run"""
        try:
          for stage_ in self._children:
            if isinstance(stage_, stage):
              if not stage_.run ():
        	self.start("error",{"#title":"Non-fatal error occured"}, a_os)
        	self.non_fatal_error += 1
        except RuntimeError, err:
          tmp_ = self.start("error",{"#title":"Fatal RuntimeError occured"}, a_os)
          print >> a_os, str(err)
          del tmp_
          self.fatal_error += 1
        except Exception, err:
          tmp_ = self.start("error",{"#title":"Fatal generic error occured"}, a_os)
          print >> a_os, str(err)
          del tmp_
          self.fatal_error += 1
        except:
          self.start("error",{"#title":"Unknown fatal error occured"}, a_os)
          self.fatal_error += 1
        
    def __init__(self):
        super(job, self).__init__()
        #The number of fatal errors detected.
        self.fatal_error = 0
        #The number of non-fatal errors detected.
        self.non_fatal_error = 0
        #The list of tests being performed
        self._test_list = []
        pass
    
class stage(property_holder):
    """Arbitrary part of a job that contains the following instruction groups:
setup     : instructions to prepare for the test
execute : the instructions to exercise the system
verify      : instructions to test output of exercise
cleanup : instructions to remove test system

Run performs the following:
* run setup verbs
* run execute verbs
* run verify verbs
* if all above passed, run cleanup verbs """
    __ctor = constructor(u"stage", lambda name, attr: stage.make(name, attr))
    cleanup_category_name = u"cleanup"
    element_name = u"stage"
    execute_category_name = u"execute"
    setup_category_name = u"setup"
    verify_category_name = u"verify"
    def cleanup(self):
        """Execute the cleanup verb list. This should remove the test suite

Typical verbs: file-list[@type=remove]"""
        self.do_category (stage.cleanup_category_name)
        
    def do_category(self, category):
        """Execute all verb-lists of a particular category."""
        if self._children:
          subtmp_ = None
          for item_ in self._children:
            if isinstance(item_, verb_list):
              if category == item_.category:
                if not subtmp_:
        	  subtmp_ = self.start("sub-stage", {"#title":"Running " + category + " stage code"})
        	  if item_.content:
        	    p_ = item_.start("par")
        	    print item_.content
        	    del p_
                tmp_ = item_.start("code",{"#title":"Code to execute"})
                print item_.get_action ()
                del tmp_
                result = True
                tmp_ = item_.start("output",{"#title":"Code output"}),
        	exec item_.get_action ()
                del tmp_
        	self.passed = result
                if not self.passed:
                  return
          if subtmp_: del subtmp_
        pass
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == stage.element_name
        return stage()
        
    def run(self):
        """Perform script. Return success (1), non-fatal failure (0) or raise
a RuntimeError for fatal errors"""
        # Run each verb list with quit on fail
        self.passed = True
        self.start ("divider")
        tmp_ = self.start("stage", {"#title":self.content or "Begin job stage"})
        for category_ in ( stage.setup_category_name, stage.execute_category_name, stage.verify_category_name ):
          self.do_category (category_)
        if self.passed:
          # Cleanup if all operations passed.
          self.do_category (stage.cleanup_category_name)
        del tmp_
        return self.passed
        
    def system_call(self, a_cmd, a_errmsg):
        """Run a system command, capturing stdout and stderr"""
        import sys, subprocess
        proc_ = subprocess.Popen(a_cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        ( res_out, res_err ) = proc_.communicate ()
        assert None == res_err, "Got unexpected output on redirected stderr: " + res_err
        sys.stdout.write(res_out)
        if proc_.returncode:
          exec a_errmsg
        pass
        
    def __init__(self):
        super(stage, self).__init__()
        #The ordered list of verbs to cleanup a passed test.
        #
        #Typical verbs: remove
        self._cleanup_list = None
        #The verbs to use to perform a test
        #
        #Typical verbs: system, run
        self._execute_verbs = None
        #Whether this stage passed or failed.
        self.passed = False
        #The list of verbs to perform for set up.
        #
        #Typical verbs: checkout, unpack, patch (filelist)
        self._setup_verbs = None
        #The ordered list of verbs to perform the verification
        #
        #Typical verbs: filelist, filecheck, compare, run
        self._verify_verbs = None
        pass
    
class test_suite(subfile_holder):
    """Main class for performing a test suite.

* This object creates a series of test sets called "job"s
* A job is a series of one or more tests that must be run serially.
* Different jobs can be run in parallel.
* The test suite is read from an XML description file.

* Dictionary of common properties can be placed in a subfile which will be
  loaded into the current scope.

<test-suite @src>
  <property @name>value</>
  <build>
    <tool @exe>args</>
    ...
  </>
  <job>...</>
  ...
</>"""
    __ctor = constructor(u"test-suite", lambda name, attr: test_suite.make(name, attr))
    element_name = u"test-suite"
    #The file to read
    file_ = option("--file",True)
    #Option to select jobs by property.
    property_ = option("--prop",True)
    #Program option to run a sub-set of jobs.
    run_ = option("--run",True)
    #Single test suite object.
    __singleton = None
    @staticmethod
    def do(manager):
        """Function to pass to each thread"""
        if _DEBUG:
          tmp_ = manager.start ("debug",{"#title":"suite content"})
          print str(manager)
          del tmp_
        while True:
          # Get a job number
          num_ = 0
          if manager._list_lock:
            with manager._list_lock:
              num_ = manager.next_job
              manager.next_job += 1
          else:
            num_ = manager.next_job
            manager.next_job += 1
          # Check if all jobs are done
          if manager.indices:
            if num_ >= len(manager.indices):
              return
          elif num_ >= manager.qsize:
            return
          #import os
          #if 0 == os.fork():
          # run job
          #dir_ = os.tempnam()
          #os.makedirs(dir_)
          #os.chdir(dir_)
          if manager.indices:
            job_num_ = manager.indices[num_]
          else:
            job_num_ = num_
          # job.run must never raise an error
          manager._children[job_num_].run (job_num_)
          #verb_filelist.remove_dirs(dir_)
          #  import sys
          #  sys.exit(0)
          #os.wait()
        pass
        
    def __init__(self):
        """# build system definitions
self.tool=[ "bjam", "jam", "make" ]
self.debug=[ "debug -d2", "-sVARIANT=DEBUG -a -d2", "VARIANT=DEBUG -t -v" ]
self.clean=[ "clean -d2", "clean -d2", "clean -v" ]
self.release=[ "release -d2", "-sVARIANT=RELEASE -a -d2", "VARIANT=RELEASE -t -v" ]
self.install=[ "", "install -sVARIANT=RELEASE -d2", "install VARIANT=RELEASE -v" ]
# other definitions
self.bouml="/usr/bin/bouml"
self.boumldir="/usr/lib64/bouml"
self.butter=self.boumldir + "/butter"
self.cpp_generator=self.boumldir + "/cpp_generator"
self.manifest="MANIFEST"
# Check values
if ! -e self.bouml:
  raise "Bouml executable not found at " + self.bouml
if ! -d self.boumldir:
  raise "Bouml directory not found at " + self.boumldir
if ! -e self.butter:
  raise "Butter plug-out not found at " + self.butter
if ! -e self.cpp_generator:
  raise "C++ generator plug-out not found at " + self.cpp_generator

"""
        #The jobs to be run
        self.indices = None
        #The index of next job to perform
        self.next_job = 0
        #The lock needed to manipulate the job cursor in
        #multithreaded running.
        self._list_lock = None
        #The size of the job queue
        self.qsize = 0
        super(test_suite, self).__init__()
        assert None == test_suite.__singleton
        test_suite.__singleton = self
        pass
        
    def index_(self):
        """Create set of indices for job run"""
        import re
        self.indices = range(self.qsize)
        if test_suite.run_.value:
          self.indices = map(int,re.findall("\d\d*", test_suite.run_.value))
        if test_suite.property_.value:
          (name, value) = test_suite.property_.value.split('=',2)
          old_index_ = self.indices
          self.indices = []
          for i in old_index_:
            if self._children[i].get(name,"") == value:
              self.indices.append(i)
        if _DEBUG:
          tmp_ = self.start("debug", {"#title":"in use job indices"})
          print self.indices
          del tmp_
        pass
        
    def main(self, fname):
        """The main function for performing the test suite.
actions:
* Read test suite description file (given on cmdline 
or stdin) to generate the set of jobs.
* Run the jobs in parallel.
* Print reports from each job at end.


# Remove out directory if it exists
if -e outdir:
  unlink (outdir)

# Create build system
system (self.bouml, project, "-exec", self.butter, "-exit")
# create source code
system (self.bouml, project, "-exec", self.cpp_generator, "-exit")

# go to build directory and check build system
olddir = cd(outdir)
if -e self.manifest:
  system ("/bin/sh", "-c", self.manifest)

# attempt to build source
test_build (self.tool[buildstyle], None)
# clean
test_build (self.tool[buildstyle], self.clean[buildstyle])
# attempt to build source
test_build (self.tool[buildstyle], self.debug[buildstyle])
# clean
test_build (self.tool[buildstyle], self.clean[buildstyle])
# release
test_build (self.tool[buildstyle], self.release[buildstyle])
# install
if self.install[buildstyle]:
  test_build (self.tool[buildstyle], self.install[buildstyle])
"""
        pass
        
    @staticmethod
    def make(elemname, elemattr):
        """Meta-constructor method"""
        assert elemname == test_suite.element_name
        test_suite.__singleton.set_src(elemattr)
        test_suite.__singleton.load_subfile()
        return test_suite.__singleton
        
    def run(self):
        fname = test_suite.file_.value
        dummy = object_interface ()
        # Read file
        if fname:
          parser_ = interpreter(dummy)
          parser_.parse (fname)
          del parser_
        assert self.parent () == dummy
        self._parent = None
        self.qsize = len(self._children)
        self.index_ ()
        if 0 < self.qsize:
          if True:
            # running single threaded
            test_suite.do (self)
          else:
            # running multi-thread
            self._dict_lock = threading.Lock ()
            self._list_lock = threading.Lock ()
            num_threads_ = 2
            thread_pool_ = []
            for n_ in range(num_threads_ - 1):
              thread_pool_.append (threading.Thread(test_suite.do, self))
            for n_ in range(num_threads_ - 1):
              thread_pool_[n_].start ()
            for n_ in range(num_threads_ - 1):
              thread_pool_[n_].join ()
          import sys
          for i_ in self.indices:
            self._children[i_].report(sys.stdout, i_)
        pass
        
    


if __name__ == "__main__":
  if not _DEBUG:
    print "<html encoding='UTF-8'><body>"
  suite = test_suite ()
  suite.run ()
  print "</body></html>"
