
#include "location.h"


// manual includes
#include "config.h"
#include <qdir.h>

namespace butter {

// -

location::location(QString path)
: children_()
, parent_(nullptr)
, path_(path)
, state_(nullptr)
{}

// -

location::location()
: children_()
, parent_(nullptr)
, path_(".")
, state_(nullptr)
{}

//Deletes all children.
location::~location() {
for(location * c : children_) delete c;
  
}

// Ctor for child location objects (a_path is relative to the parent
// location.) Not to be called directly, use "create_as_child".
location::location(QString a_path, location & a_parent)
: children_()
, parent_(&a_parent)
, path_(a_path)
, state_(nullptr)
{
a_parent.children_.push_back(this);
}

//The path from the parent location, this is an empty string for the top-level location
//
//(\see location_set::offset_path and \see location::full_path)
QString location::full_path() const {
enumerate_path result(this->path_);
// Process locations from here to root
for (const butter::location * tmp_ = this->parent_;
	tmp_;
	tmp_ = tmp_->parent_)
{
  enumerate_path lhs_( tmp_->path_ );
  result = lhs_ / result;
}
return QDir::cleanPath(result.path());
}

//Default constructor (src_dir_ is set to '.')
location_set::location_set()
: src_dir_()
, root_()
{}

//Default constructor, single argument is the CppSrcDir from bouml.
location_set::location_set(QString src_dir)
: src_dir_(src_dir)
, root_()
{}

location_set::~location_set() {}

//The path to the root of the build system. All locations in the set are
//below this point. Note this may not be the same as the src_dir which
//is defined in the bouml project.
//

QString location_set::base_path() const {
  return (root_  
    ? root_->path()
    : src_dir_.path());
}

//Attempt to find the closest location to a_path.
//
//\pre root.path.fullpath.has_subpath(a_path)
//
//\result - return nul if location_set is empty
//           - result.path.fullpath.has_subpath(a_path)

location * location_set::find_closest(const enumerate_path & a_path) const {
  if( ! root_ ) return nullptr;
  return find_closest_( src_dir_ / a_path );
}

//Attempt to find the closest location to a_path.
//
//\pre root.path.fullpath.has_subpath(a_path)
//
//\result - return nul if location_set is empty
//           - result.path.fullpath.has_subpath(a_path)

location * location_set::find_closest_(const enumerate_path & a_path) const {
  BUTTER_ALWAYS( root_.get() != nullptr, "Should never call private find_closest_ with nul root" );
  butter::enumerate_path const parent_path_{ root_->path() };
  butter::enumerate_path const search_path_{ a_path }; // path already adjusted for src_dir
  BUTTER_REQUIRE(parent_path_.has_subpath( search_path_ ), "This location is not a parent of the given path");
  butter::enumerate_path const diff_path_( parent_path_.create_relative( search_path_ ));
  location * cursor_ = root_.get();
  for( unsigned int i_ = 0; i_ < diff_path_.depth(); ++i_ )
  {
    butter::location * tmp_ = NULL;
    for( location * kid : cursor_->children_ )
    {
      if( kid->path_ == diff_path_.leaf_at(i_) )
      {
        tmp_ = kid;
        break;
      }
    }
    if( NULL == tmp_ )
    {
      break; // At closest point
    }
    else
    {
      cursor_ = tmp_;
    }
  }
  return cursor_;
}

//Return a pointer to the root location, may be nul
const location * location_set::root() const {
  return root_.get();
}

//Get the sequence of locations in top-down order.
QVector< location * > location_set::topdown() const {
  QVector< location * > v;
  if(this->root_)
  {
    location * loc{ this->root_.get() };
    // build vector
    int idx{ 0 };
    v.push_back(loc);
    while( idx != v.size() )
    {
      if( ! v[idx]->children_.empty() )
      {
        for( location * a : v[idx]->children_ )
        {
          v.push_back(a);
        }
      }
      ++idx;
    }
  }
  return v;
}

//This descends the location tree writing debug information to
//a_os as it goes.
//
//(Only if NO_LOG is undefined)
void location_set::serialize(QTextOStream & a_os) const {
#ifndef DEBUG
#define DEBUG 1
#endif
BUTTER_ALWAYS(DEBUG, "Programming error: attempt to get debug info in release build");
if (DEBUG)
{
  if(! root_)
  {
    a_os << "EMPTY location set.\n";
    return;
  }
  QVector< const location * > stack_;
  stack_.push_back(root_.get());
  while(! stack_.isEmpty())
  {
    location const * top_ = stack_.front();
    stack_.pop_front();
    a_os << "<<BEGIN location [" << top_->full_path() << "]\n";
    for(location const * loc_ : top_->children())
    {
      a_os << "                |- [" << loc_->path() << "]\n";
      if(! loc_->children().isEmpty())
      {
        stack_.push_back(loc_);
      }
    }
    a_os << "<<  END location [" << top_->full_path() << "]\n";
  }
}

}

//Find or create a new location object that matches the given
//path.  If new locations are created, one location is created
//for each level of the path that is a sub-path of offset_path.
//
//It is not an error to provide a path with an existing matching
//path.
//
//
//\post new.offset_path.equality(old.offset_path) or new.offset_path.has_subpath(old.offset_path)
//\post result.path = search_path.leaf_at(search_path.depth - 1)

location * location_set::insert(QString a_path) {
const enumerate_path search_path_{ src_dir_ / a_path };
if(! root_)
{
  // Empty root so this is the current "root" location
  root_.reset(new location(search_path_.path()));
  return root_.get();
}
location * cursor_ = root_.get();
enumerate_path cursor_path_{ root_->path() };
if( ! cursor_path_.equality(search_path_) )
{
  if ( ! cursor_path_.has_subpath(search_path_) )
  {
    // Need to reparent root
    // * either : old root is subdirectory of the search directory
    // * or     : need new common parent (exception if no common parent possible)
    enumerate_path new_root_path_{ search_path_.has_subpath(cursor_path_)
      ? search_path_
      : src_dir_ / src_dir_.create_relative( cursor_path_.create_common( search_path_ )) };
    std::unique_ptr< location > new_root_{ new location(new_root_path_.path()) };
    // Make the old root location a child of the new root location or its children
    cursor_path_ = new_root_path_.create_relative(cursor_path_);
    cursor_ = new_root_.get();
    for( unsigned int i_ = 0; i_ < cursor_path_.depth() - 1; ++i_ )
    {
     cursor_ = new location(cursor_path_.leaf_at(i_), *cursor_ );
    }
    // change old root's path and parent
    root_->path_ = cursor_path_.leaf_at(cursor_path_.depth() - 1);
    root_->parent_ = cursor_;
    cursor_->children_.push_back(root_.release());
    // Set the new root location
    root_.reset(new_root_.release());
    // New root path is
    // * either : == search path -> job completed
    // * or     : parent of search path -> build descendent
    cursor_ = root_.get();
    cursor_path_ = root_->path();
  }
  else
  {
    // Need to find closest existing location. Path is
    // * either : == search path -> job completed
    // * or     : parent of search path -> build descendent
    cursor_ = find_closest_( search_path_ );
    cursor_path_ = cursor_->full_path();
  }
  if ( ! cursor_path_.equality( search_path_ ) )
  {
    // New root is parent of search path. Build descendent
    enumerate_path const diff_path_( cursor_path_.create_relative( search_path_ ) );
    for( unsigned int i_ = 0; i_ < diff_path_.depth(); ++i_ )
    {
      cursor_ = new location(diff_path_.leaf_at(i_), *cursor_ );
    }
  }
}
return cursor_;
}


} // namespace butter
