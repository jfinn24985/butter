#ifndef BUTTER_LOCATION_H
#define BUTTER_LOCATION_H


#include <qvector.h>
#include <qstring.h>
#include "utility.h"
#include <memory>
#include <qtextstream.h>

namespace butter {

//Manages a build location in the project. It has a one-to-one correspondence with a filesystem 
//directory and one-to-many with the set of UmlPackages for the directory. These are generated in the first
//phase of the program execution.
//
//Responsibilities:
//- Property handling
// - base
// - log-level
// - style
//
//Location objects form a tree like structure that parallels a filesystem 
//directory structure, except locations may be sparse.
//
//Axioms:
//
//1) There is exactly one root location containing all other locations
//
//2) A location can have children more than one level deep in the filesystem
//
//3) A location can not have a child A that's directory is a 
//  subdirectory of child B. { If child B exists when child A is added then
//  it is forwarded to child B. If child A exists when child B is added then
//  child A is relocated to child B }
//
//Example 1:
//
//.../ rootpath <*> / subpath1     / subpathA     / subpathX <*>
//                  / subpath2 <*> / subpathB <*>
//
//gives
//
//[location:rootpath].children{ [location:subpathX], 
//    [location:subpath2].children{ [location:subpathB] } }
//
//Example 2:
//
//.../ rootpath1 <*>
//.../ rootpath2 <*>
//
//gives
//
//[location:common_parent(rootpath1,rootpath2)].children{ 
//    [location:rootpath1], [location:rootpath2] }
//
//

class location {
  friend class location_set;

  private:
    //The child nodes (managed)
    
    QVector<location*> children_;

    location * parent_;

    //* Relative path from parent to this location. If no parent then is the "location_base_path"@.
    //* Path may not yet exist on filesystem.
    //
    //@location_base_path is defined as the relative path to the base_path location in the bouml project.
    //The bouml base_path location is either relative to the project working directory or absolute. We 
    //assume QDir::exists(base_path) or QDir::mkdir(base_path) will succeed and therefore so will
    //QDir::exists(location_base_path) or QDir::mkdir(location_base_path).
    QString path_;

    //An opaque structure used by the butter system to manage build system creation.
    void * state_;


  public:
    //Ctor for location tree root objects
    location(QString path);
    //Ctor for location tree root objects
    location();
    //Deletes all children.
    ~location();

    // Ctor for child location objects (a_path is relative to the parent
    // location.) Not to be called directly, use "create_as_child".
    location(QString a_path, location & a_parent);

  private:
    //no copy
    location(const location & source) = delete;
    //no assign
    location & operator=(const location & source) = delete;

  public:
    //Get the children!
    const QVector< location * > & children() const {
       return children_;
    }
    //The path from the parent location, this is an empty string for the top-level location
    //
    //(\see location_set::offset_path and \see location::full_path)
    QString full_path() const;
    //The parent location of this location, or nul for the top-level location.
    const location * parent() const {
       return parent_;
    }
    //The path to the root location of the containing location set.
    //
    //(\see location_set::offset_path and \see location::full_path)
    const QString & path() const {
      return path_;
    }
    //The value content of this location
    const void * value() const {
      return state_;
    }
    //Set the value content of this location
    void value(void * val) {
      state_ = val;
    }
    //The value content of this location
    void * value() {
      return state_;
    }
};
//Container for the set of locations that will be used in defining the build system.
//
//
//Programming notes: Paths and locations
//
//* All paths as arguments to methods are directly from bouml
//
//* Relative paths from the bouml project are relative to CppSrcDir.
//
//* Each subdirectory (with content) in the source tree has a corresponding location object.
//
//* The root location may be above the directory pointed to by CppSrcDir, so always incorporates the CppSrcDir in its path.
//
//    * NB. If the root directory does not incorporate the CppSrcDir then path manipulation gets polluted with local paths
//

class location_set {
  private:
    //CPP source directory defined in the project.  All paths
    //non-absolute paths will be relative to this location.
    //
    //* All paths as arguments to methods are directly from bouml
    //
    //* Relative paths from the bouml project are relative to CppSrcDir.
    
    enumerate_path src_dir_;

    //The root of the location set
    //
    //* The root location may be above the directory pointed to by CppSrcDir, so always incorporates the CppSrcDir in its path.
    //
    //    * NB. If the root directory does not incorporate the CppSrcDir then path manipulation gets polluted with local paths
    std::unique_ptr<location> root_;


  public:
    //Default constructor (src_dir_ is set to '.')
    location_set();

    //Default constructor, single argument is the CppSrcDir from bouml.
    location_set(QString src_dir);

    ~location_set();


  private:
    location_set(const location_set & source) = delete;

    location_set(location_set && source) = delete;

    location_set & operator=(const location_set & source) = delete;


  public:
    //The path to the root of the build system. All locations in the set are
    //below this point. Note this may not be the same as the src_dir which
    //is defined in the bouml project.
    //
    
    QString base_path() const;

    //Attempt to find the closest location to a_path.
    //
    //\pre root.path.fullpath.has_subpath(a_path)
    //
    //\result - return nul if location_set is empty
    //           - result.path.fullpath.has_subpath(a_path)
    
    location * find_closest(const enumerate_path & a_path) const;


  private:
    //Attempt to find the closest location to a_path.
    //
    //\pre root.path.fullpath.has_subpath(a_path)
    //
    //\result - return nul if location_set is empty
    //           - result.path.fullpath.has_subpath(a_path)
    
    location * find_closest_(const enumerate_path & a_path) const;


  public:
    //Return a pointer to the root location, may be nul
    const location * root() const;

    //CPPSrcDir in bouml project.
    const enumerate_path & source_directory() const {
       return src_dir_;
    }

    //Get the sequence of locations in top-down order.
    QVector< location * > topdown() const;

    //This descends the location tree writing debug information to
    //a_os as it goes.
    //
    //(Only if NO_LOG is undefined)
    void serialize(QTextOStream & a_os) const;
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
    
    location * insert(QString a_path);
};

} // namespace butter
#endif
