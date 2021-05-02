#ifndef BUTTER_UTILITY_H
#define BUTTER_UTILITY_H


#include <qdir.h>

#include <qstring.h>

namespace butter {

//Treat filesystem paths as a list of filesystem folder names.
//This extends the Qt QDir class to allow paths to be manipulated.
//Methods for path manipulation avoid operations that reference
//the real operating system filesystem.  This means that distinct
//paths are assumed to never refer to the same location, which
//is generally valid unless the paths involve symlinks.

class enumerate_path : public ::QDir {
  private:
    //Separator to use regardless of OS
    
    static const char default_separator_[2];

    //separator used on other OS
    
    static const char other_separator_[2];


// Lifetime methods


  public:
    //Default ctor
    enumerate_path(): QDir() {}
    //Construct from string (automatically normalizing the path)
    enumerate_path(QString a_path): QDir(normalise(a_path)) {}
    //Construct from base class (automatically normalizing path)
    enumerate_path(const QDir & source): QDir(normalise(source.path())) {}
    virtual ~enumerate_path() {}
    enumerate_path(const enumerate_path & source): QDir(source) {}
    enumerate_path(enumerate_path && source): QDir(std::move(source)) {}
    enumerate_path & operator =(enumerate_path source) {
       this->setPath(source.path());
       return *this;
    }
    void swap(enumerate_path & other);


// Access methods


  private:
    //Test if the path starts with a drive letter as in "?:"
    bool is_dos_absolute() const;


  public:
    //The number of steps in the path.
    unsigned int depth() const;

    //Here subpath means that a_rhs is a subdirectory or file in this
    //directory.
    //
    //NOTE: a is_subpath implies that this == create_common(a_rhs).
    
    bool has_subpath(const enumerate_path & a_rhs) const;
    //Compare two paths. Returns canonicalPath = a_rhs.canonicalPath 
    //(ignoring trailing separator)
    bool equality(const enumerate_path & a_rhs) const;
    //Get part of path at position a_count
    QString leaf_at(unsigned int a_count) const;
    friend bool operator ==(const enumerate_path & a_lhs, const enumerate_path & a_rhs) {
      return a_lhs.equality (a_rhs);
    };
  using QDir::path;  using QDir::exists;  using QDir::isRelative;    //Generate a path that has the greatest shared path between this
    //and a_other.
    //
    //Will raise and exception when paths are on different drives.
    //
    //**NOTE: This class is not intended as a general solution.
    //In the butter program having two paths that have no common root
    //is an exception as we can not create a build system without
    //the common root.  This is why this method throws an exception**
    
    QString create_common(const enumerate_path & a_other) const;
    //Build a relative path from here to a_target.
    //
    //canonical (path_ + relative (a_target)) <==> a_target
    QString create_relative(const enumerate_path & a_target) const;

// Modification methods

    //Convert string to form with all separators as "/" (function part)
    static QString normalise(QString a_path);
    //Concatenate 2 paths
    enumerate_path operator /(const enumerate_path & rhs) const {
       return enumerate_path(path () + default_separator_ + rhs.path ());
    }
    //Concatenate 2 paths
    enumerate_path operator /(QString rhs) const {
       return enumerate_path(path () + default_separator_ + normalise (rhs));
    }
    //Output path using the current OS'S separators.  Used when
    //writing paths for external visibility.
    QString path_localised() const {
          return QDir::convertSeparators (path ());
        };
    //Create a copy of the path string that has all the separators replaced 
    //by some arbitrary string "a_trans".
    QString path_convert(QString a_trans) const;
    //Change to a new internal path (automatically normalize)
    void setPath(const QString & a_path) {
       QDir::setPath (normalise (a_path));
    }

// Modify filesystem methods

    //Create this and all parent directories for the current path.
    bool mkpath() const;
};

} // namespace butter
#endif
