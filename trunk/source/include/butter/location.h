#ifndef BUTTER_LOCATION_HPP
#define BUTTER_LOCATION_HPP


#include "butter/utility.h"
#include <qvector.h>
#include <qstring.h>
#include <memory>
#include <qtextstream.h>


class CppSettings;
namespace butter { class basic_style; } 
namespace butter { class butter_constants; } 
namespace butter { class log; } 
namespace butter { class style; } 
class UmlPackage;
class UmlArtifact;
class UmlItem;

namespace butter {

/**
 * Manages a build location in the project. It has a one-to-one correspondence with a filesystem 
 * directory and one-to-many with the set of UmlPackages for the directory. These are generated in the first
 * phase of the program execution.
 * 
 * Responsibilities:
 * - Property handling
 *  - base
 *  - log-level
 *  - style
 * 
 */
class location {
  private:
    /**
     * The child nodes (managed)
     */
    QVector< location > children_;

    /**
     * The packages associated with this location,
     */
    QVector< ::UmlPackage > packages_;

    /**
     * The parent
     */
    location * parent_;

    /**
     * The location's filesystem path.
     */
    pathcmp path_;


  public:
    /**
     * Add the package as a client of the location.
     * 
     * \pre path of a_package = full_path
     */
    void add_package(const ::UmlPackage & a_package);

    /**
     * Get the children!
     */
    const QVector< location >& children() const
    {
      return children_;
    }


  private:
    /**
     * Are the contents of the file a_path and a_str identical? If they are not then
     * write a_str to a_path and return false. Throws an exception if it cannot open a file
     * at a_path.
     */
    static bool compare_file_to_string(QString a_path, QString a_str);


  public:
    /**
     * Create a new location object as a descendent of this location.  The
     * created object is owned by this object or one of its descendents.
     * 
     * \pre full_path.has_subpath( a_path )
     */
    location * create_as_child(const pathcmp & a_path);


  private:
    /**
     * Create a location object that is a parent of the a_base 
     * location and a_path. Only base location objects may call 
     * this method. The pointer a_base should refer to allocated
     * memory which the method takes over management of. The
     * return is either a new root location or a_base. The
     * returned pointer should be managed by the caller.
     * 
     * ** This reroots the location tree. The returned object
     * is the new root of the location tree. **
     * 
     * \pre nul = a_base.parent
     */
    static std::unique_ptr< location > create_common_parent(std::unique_ptr< location > a_base, const pathcmp & a_path);


  public:
    /**
     * Create a document at the current location with  a_name. This 
     * scans the package(s) for a deployment view and uses the first one 
     * found.  If none are found then it creates a deployment view in
     * the first package.  Then it creates an artifact with a_name in the
     * selected deployment view.
     * 
     * \pre not packages.empty
     * \pre not has_document(a_name)
     */
    ::UmlArtifact * create_uml_document(QString a_name);

    /**
     * Attempt to find a location pointer for the given path. Will return the "closest" 
     * location if no exact match is found.
     * 
     * \pre a_location.parent_path (a_path)
     */
    static location * find(location * a_location, const pathcmp & a_path);

    /**
     * Find a document of the given name at the current location. This
     * may return an empty list if no document is found
     * 
     * \pre not a_name.empty
     */
    QVector< ::UmlItem > find_uml_document(QString a_name) const;

    /**
     * This gives the fully qualifed path of the location (relative to
     * the base path.)
     */
    pathcmp full_path() const;
    /**
     * Ctor for location tree root objects (a_path is full
     * path or relative to a base path.)
     */
    location(const pathcmp & a_path);


  private:
    /**
     *  Ctor for child location objects (a_path is relative to the parent
     *  location.) Not to be called directly, use "create_as_child".
     */
    location(const pathcmp & a_path, location & a_parent);


  public:
    /**
     * Deletes all children.
     */
    ~location() throw ();


  private:
    /**
     * no copy
     */
    location(const location & source) = delete;

    /**
     * no assign
     */
    location & operator=(const location & source) = delete;


  public:
    /**
     * The packages associated with this location
     */
    const QVector< ::UmlPackage >& packages() const
    {
      return packages_;
    }

    /**
     * The parent location, this is nul for the top-level locations
     */
    location *const parent() const
    {
      return parent_;
    }

    /**
     * This visits all the UmlItems starting at (a_item) and proceeding
     * through all of the descendent items.  It returns a location tree
     * suitable for creating a build-system.  The return value may be empty
     * in the event of an error or there being no targets or documents.
     * 
     * \arg a_item : Root of bouml tree.
     * \arg a_interaction : automate user interaction (during test mode)
     *   (0 = query user(default), 1 = user selects "Ok", 2 = user selects other option)
     * 
     * \pre a_item.type = UmlPackageType and nul = a_item.parent [ie = project]
     */
    static std::unique_ptr<location> parse_project(::UmlItem & a_item, unsigned a_interaction);

    /**
     * The leaf node of sub-locs and the base-path for top-level locs
     */
    const pathcmp& path() const
    {
      return path_;
    }

    /**
     * This descends the location tree writing debug information to
     * a_os as it goes.
     * 
     * (Only if NO_LOG is undefined)
     */
    void serialize(::QTextOStream & a_os) const;
    /**
     * Write all UML documents for this and descendent locations.
     * 
     * * This calls write_uml_documents on a_base and all a_base's descendent
     * locations.
     */
    static void write_documents(const location & a_base);

    /**
     * Scan the list of packages at this location and write any UmlArtifact:documents 
     * to the file-system.
     */
    void write_uml_documents() const;

};


} // namespace butter
#endif
