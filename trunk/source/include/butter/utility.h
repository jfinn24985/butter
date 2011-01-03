#ifndef BUTTER_UTILITY_HPP
#define BUTTER_UTILITY_HPP

/**
 * Utility classes log and pathcmp
 */
#include <qdir.h>

#include <qstring.h>
#include <memory>
#include <qfile.h>
#include <qtextstream.h>


class UmlCom;

namespace butter {

/**
 * Minimal class to allow comparison of file paths as ordered lists of strings.
 * 
 * The methods defined in this derived class allow some operations that do
 * not reference the operating system.  This means that comparisons etc
 * are for absolute paths without resolving symlinks.
 */
class pathcmp : public ::QDir
{
  public:
    /**
     * Generate a path that has the greatest shared path between this and a_other.
     * 
     * Will returnempty string when paths are on different drives.
     */
    QString create_common(const pathcmp & a_other) const;

    /**
     * Build a relative path from here to a_target.
     * 
     * canonical (path_ + relative (a_target)) <==> a_target
     */
    QString create_relative(const pathcmp & a_target) const;

    /**
     * The number of steps in the path.
     */
    unsigned int depth() const
    {
      const QString path_ (path ());
      return (unsigned int)(path_.contains (default_separator_[0]) + (path_.at(path_.length () - 1) == default_separator_[0] ? 0 : 1));
    }

    /**
     * Compare two paths. Returns canonicalPath = a_rhs.canonicalPath 
     * (ignoring trailing separator)
     */
    bool equality(const pathcmp& a_rhs) const;

    /**
     * Here subpath means that a_rhs is a subdirectory or file in this
     * directory.
     * 
     * NOTE: a is_subpath implies that this == create_common(a_rhs).
     * 
     */
    bool has_subpath(const pathcmp & a_rhs) const;

    /**
     * Get part of path at position a_count
     */
    QString leaf_at(unsigned int a_count) const;

    /**
     * Create this and all parent directories for the current path.
     */
    bool mkpath() const;


  private:
    /**
     * Convert string to form with all separators as "/" (inline part)
     */
    static QString normalise(QString a_path)
    {
      return -1 == a_path.find (other_separator_) ? a_path : normalise_(a_path);
    }

    /**
     * Convert string to form with all separators as "/" (function part)
     */
    static QString normalise_(QString a_path);

  public:
    pathcmp & operator=(pathcmp source)
    {
      setPath (source.path ());
      return *this;
    }

    /**
     * Concatenate 2 paths
     */
    pathcmp operator/(const pathcmp & rhs) const
    {
      return pathcmp(path () + default_separator_ + rhs.path ());
    }

    /**
     * Concatenate 2 paths
     */
    pathcmp operator/(QString rhs) const
    {
      return pathcmp(path () + default_separator_ + normalise (rhs));
    }

friend bool operator==(const pathcmp &a_lhs, const pathcmp &a_rhs)
{
  return a_lhs.equality (a_rhs);
}    /**
     * Output path using the current OS'S separators.  Used when
     * writing paths for external visibility.
     */
    QString path_localised() const
    {
      return QDir::convertSeparators (path ());
    }
    /**
     * Create a copy of the path string that has all the separators replaced 
     * by some arbitrary string "a_trans".
     */
    QString path_convert(QString a_trans) const;

    /**
     * Default ctor
     */
    pathcmp()
    : QDir ()
    {}

    /**
     * Construct from string
     */
    pathcmp(QString a_path)
    : QDir (normalise (a_path))
    {}

    ~pathcmp()
    {}

    pathcmp(const pathcmp & source)
    : QDir (source)
    {}

    /**
     * Construct from base class
     */
    pathcmp(const ::QDir & source)
    : QDir (normalise (source.path ()))
    {}

    void setPath(const QString& a_path)
    {
      QDir::setPath (normalise (a_path));
    }


  private:
    /**
     * Separator to use regardless of OS
     */
    static const char default_separator_[2];

    /**
     * separator used on other OS
     */
    static const char other_separator_[2];


};

/**
 * Central point for handling logging and debugging messages.
 */
class log
{
  public:
    /**
     * Logging levels
     */
    enum log_levels
    {
      warn = 0,/**
       * Base logging level, reports warning and error messages.
       */

      info = 1,/**
       * Include information messages
       */

      debug = 2/**
       * include debugging messages
       */


    };

    /**
     * Exemplar
     */
    static log com;

  private:
    /**
     * File for debugging information. 
     */
    std::auto_ptr< QFile > debug_file_;

    /**
     * Output stream for debug builds. 
     */
    std::auto_ptr< ::QTextOStream > debug_os_;

    /**
     * The current logging level. (default is 'warn')
     */
    log_levels level_;


  public:
    /**
     * Open a_fname file for debug messages. (Only present with debug builds.)
     */
    void debug_log(const char * a_fname);

    /**
     * Is it possible to write debug messages.
     */
    bool is_debug() const
    {
      return DEBUG && NULL != debug_os_.get ();
    }

    /**
     * Get the logging level
     */
    int level() const
    {
      return level_;
    }

    /**
     * Set the logging level
     */
    void level(int a_val)
    {
      level_ = (0 >= a_val ? warn : (2 <= a_val ? debug : info ));
    }
        


  private:
    log()
    : debug_file_ ()
    , debug_os_ ()
    , level_(warn)
    {}


  public:
    /**
     * Dtor to ensure correct order of deconstructing stream and file
     */
    ~log();

    /**
     * Get reference to debug stream. (Only present with debug builds.)
     * 
     * Calling this method before debug_log(filename) is undefined. This can
     * be tested with is_debug
     * \pre is_debug
     */
    ::QTextOStream & stream() const;
    /**
     * Write a message to the trace window
     */
    void trace(log_levels a_lvl, const char * a_msg);


};


} // namespace butter
#endif
