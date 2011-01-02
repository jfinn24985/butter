#ifndef BUTTER_WRITER_HPP
#define BUTTER_WRITER_HPP

/**
 * Intermediate writer template class.
 */
#include "butter/build_writer.h"
#include "bouml/CppSettings.h"
#include "butter/item_traits.h"
#include "butter/compound_artifact.h"
#include "butter/style.h"
#include "bouml/UmlArtifact.h"
#include "bouml/UmlPackage.h"
#include "butter/location.h"
#include "bouml/UmlItem.h"

// Manual includes
// #include <qfile.h>
#include <qregexp.h>
#include <stdexcept>
#include <qstack.h>

// for getenv method
#include <cstdlib>
// --
namespace butter {

/**
 * Writer class for recursive template idiom.  This contains the implementation for
 * the create_system () factory method. Derived class writers can use this class to handle
 * the boiler-plate process of traversing the location and artifact sets - they implement
 * descendent_link, external_target, initialise and local_target.
 */
template<class derived>
class writer : public build_writer
{
  public:
    /**
     * DRIVER METHOD
     * 
     * Create a series of build-system artifacts based at a_base location. The
     * artifacts are inserted into a package at each location.
     */
    void create_system(location & a_base, const ::UmlItem & a_project);

    /**
     * Create the initial rules set.
     */
    void rules_file(location & a_base, const ::UmlItem & a_project);


};

template<class derived>
void writer<derived>::create_system(location & a_base, const ::UmlItem & a_project) 
{
// Check for top-level system build-file document, create it from template.
this->rules_file (a_base, a_project);

// Start building system
std::auto_ptr< compound_artifact > system_artifact_ = get_artifact (a_base, static_cast< derived* >(this)->build_file_sysname);

// Set the project root directory
this->root_dir (a_base.full_path ());
BUTTER_CHECK (! this->root_dir ().path ().isEmpty ()
      , "<p><b>Program error:</b> Project root directory is not set properly.</p>");
// Perform style specific initialisation
static_cast< derived* >(this)->initialise (a_base, a_project, *system_artifact_);

//////////////
// Process the project, handling items as we go.
//

QStack< location > location_stack_;
location_stack_.push (&a_base); // Use a stack to eliminate recursion
while (! location_stack_.isEmpty ())
{
  location * top_ = location_stack_.pop ();
  // Push children onto stack
  for (unsigned int ii_ = 0; ii_ < top_->children ().count (); ++ii_)
  {
    location_stack_.push (top_->children ().at (ii_));
  }

  std::auto_ptr< compound_artifact > current_; // The current build file.
  // Get deployment views from current location's packages
  for (unsigned int i_ = 0; i_ < top_->packages ().count (); ++i_)
  {
    UmlPackage * I_ = top_->packages ().at (i_);
    for (unsigned int j_ = 0; j_ < I_->children ().count (); ++j_)
    {
      UmlItem * J_ = I_->children ().at (j_);
      if (aDeploymentView == J_->kind ())
      {
        // Scan for deployment views targets.
        for (unsigned int k_ = 0; k_ < J_->children ().count (); ++ k_)
        {
          UmlItem * item_ = J_->children ().at (k_);
          if (anArtifact == item_->kind ())             // UmlArtifacts.
          {
            const UmlArtifact &art_item_ = dynamic_cast < UmlArtifact& >(*item_);
            const QString stereotype_label_ (const_cast< UmlArtifact& >(art_item_).stereotype ());
            bool installable_ (false);
            target_type target_type_ (executable); // The target's type, set to exe then change if library etc
            if (stereotype_label_.contains (item_traits::executable_stereotype, false)
                || stereotype_label_.contains (item_traits::library_stereotype, false))
            {
              installable_ = true;
              QString value_;
              if (art_item_.property_value (item_traits::butter_project_name, value_))
              {
                //////////////////////////////////////////////////
                // Is an external target for project 'value_'.
                static_cast< derived* >(this)->external_target (*top_, art_item_, *system_artifact_);
              }
              else
              {
                //////////////
                // Is a local target
                const QString target_name_ (art_item_.name ());
                QVector < UmlArtifact > assoc_arts_ (const_cast< UmlArtifact& >(art_item_).associatedArtifacts ());
                // Only process targets with associated entries.
                if (! assoc_arts_.isEmpty ())
                {
                  QString entry_;           // The target entry
                  {
                    if (stereotype_label_.contains (item_traits::library_stereotype, false))
                    {
                      QString value_;
                      if (art_item_.property_search (item_traits::butter_lib_type_name, value_)
                            && value_ == item_traits::shared_value)
                      {
                        target_type_ = shared_library;
                      }
                      else
                      {
                        target_type_ = static_library;
                      }
                      if (art_item_.property_value (item_traits::butter_other_name, value_))
                      {
                        target_type_ = other;
                      }
                    }
                    QString compiler_, includes_, compflags_, ldflags_;
                    art_item_.property_value (item_traits::butter_compiler_name, compiler_);
                    QTextOStream entry_os_ (&entry_);
                    ///////////////////
                    // Indicate the start of the target entry.
                    {
                      QString buildfile_;
                      art_item_.property_value (item_traits::butter_buildfile_name, buildfile_);
                      static_cast< derived* >(this)->start_target (art_item_, entry_os_, buildfile_, compiler_, target_type_);
                    }
                    ///////////////////
                    // Process the associated artifacts
                    for (unsigned int i_ = 0; i_ < assoc_arts_.size (); ++i_)
                    {
                      UmlArtifact *const current_art_ = assoc_arts_.at (i_);
                      const QString stereotype_ (current_art_->stereotype ().data ());
                      if (stereotype_ == item_traits::library_stereotype)
                      {
                        static_cast< derived* >(this)->assoc_library (*current_art_, entry_os_
                                            , includes_, ldflags_, compflags_);
                      }
                      else if (stereotype_ == item_traits::source_stereotype)
                      {
                        QString src_inc_, src_flags_;
                        find_hdr_link (*current_art_, src_inc_, ldflags_, src_flags_, static_cast< derived* >(this)->section_name, true); // Pass link flags to target
                        static_cast< derived* >(this)->assoc_source (*current_art_, entry_os_
                                            , current_art_->name () + "." + CppSettings::sourceExtension ()
                                            , current_art_->name ()
                                            , src_inc_
                                            , src_flags_
                                            , false);
                      }
                      else if (stereotype_ == item_traits::document_stereotype)
                      {
                        QString src_inc_, src_flags_;
                        find_hdr_link (*current_art_, src_inc_, ldflags_, src_flags_, static_cast< derived* >(this)->section_name, true); // Pass link flags to target
                        QString basename_ (current_art_->name ());
                        const int dot_ (basename_.findRev ('.'));
                        if (-1 != dot_) basename_.truncate (dot_);
                        static_cast< derived* >(this)->assoc_source (*current_art_, entry_os_
                                            , current_art_->name ()
                                            , basename_
                                            , src_inc_
                                            , src_flags_
                                            , true);
                      }
                    }
                    ////////////////////
                    // End target
                    find_hdr_link (art_item_, includes_, ldflags_, compflags_, static_cast< derived* >(this)->section_name, false); // Pass link flags to target
                    static_cast< derived* >(this)->end_target (art_item_, entry_os_
                                            , includes_, ldflags_, compflags_, compiler_, target_type_);
                  }
                  if (! entry_.isEmpty ())
                  {
                    if (NULL == top_->parent ())
                    {  // Is local at base, use system_artifact_
                      system_artifact_->target (target_name_).second = entry_;
                    }
                    else
                    {
                      if (NULL == current_.get ())
                      {
                        current_ = get_artifact (*top_, static_cast< derived* >(this)->build_file_name);
                      }
                      current_->target (target_name_).second = entry_;
                    }
                  }
                }
              }
            }
            //////////////
            // Handle installables
            if (installable_ || stereotype_label_.contains (item_traits::document_stereotype, false))
            {
              QString entry_;           // The install entry
              {
                QString install_dir_;              // Location for install: DOCDIR,BINDIR etc
                install_type install_type_ (build_writer::file); // How the target should be installed
                // Get the install property
                art_item_.property_value (item_traits::butter_install_name, install_dir_);
                // For libraries and executables define some defaults.
                if (stereotype_label_ == item_traits::library_stereotype)
                {
                  // Check to see if this is an external library
                  QString project_;
                  if (art_item_.property_value (item_traits::butter_project_name, project_))
                  {
                    install_dir_.truncate (0);
                  }
                  else
                  {
                    install_type_ = build_writer::lib;
                    if (install_dir_.isEmpty ())
                    {
                      install_dir_ = item_traits::libdir_value;
                    }
                    else
                    {
                      /////////////
                      // Reset install type to file to handle "other" targets
                      if (install_dir_ != item_traits::libdir_value
                         && target_type_ == build_writer::other)
                      {
                        install_type_ = build_writer::file;
                      }
                    }
                  }
                }
                else if (stereotype_label_ == item_traits::executable_stereotype)
                {
                  install_type_ = build_writer::bin;
                  if (install_dir_.isEmpty ())
                  {
                    install_dir_ = item_traits::bindir_value;
                  }
                }
                if (! install_dir_.isEmpty () && install_dir_ != item_traits::no_install_value)
                {
                  QTextOStream entry_os_ (&entry_);
                  static_cast< derived* >(this)->install_target (art_item_, entry_os_, install_dir_, install_type_, ! installable_);
                }
              }
              if (! entry_.isEmpty ())
              {
                if (NULL == top_->parent ())
                {  // Is local at base, use system_artifact_
                  system_artifact_->target (art_item_.name ()).second.append (entry_);
                }
                else
                {
                  if (NULL == current_.get ())
                  {
                    current_ = get_artifact (*top_, static_cast< derived* >(this)->build_file_name);
                  }
                  current_->target (art_item_.name ()).second.append (entry_);
                }
              }
            }
          }
        }
      }
    }
  }

  // End of current location
  if (NULL != current_.get ())
  {
    // add the inter-file link
    static_cast< derived* >(this)->descendent_link (*current_.get (), *system_artifact_, *top_);
    // Close the build file
    // MSC_NO_RESET(current_, std::auto_ptr< compound_artifact >)();
  }
}


}

template<class derived>
void writer<derived>::rules_file(location & a_base, const ::UmlItem & a_project) 
{
// Helper functions for managing rules files.
class helper_
{
public:
  static QString make_substitutions(QString content, const QString& project_name, const QString& project_version)
  {
    const QString project_placeholder_ ("@@project@@");
    const QString version_placeholder_ ("@@version@@");
    int index_ = 0;
    if (-1 != (index_ = content.find (project_placeholder_)))
    {
      content.replace (index_, project_placeholder_.length (), project_name);
    }
    if (-1 != (index_ = content.find (version_placeholder_)))
    {
      content.replace (index_, version_placeholder_.length (), project_version);
    }
    return content;
  }
  static QString list_to_string (const char *a_list[], unsigned int index)
  {
    QString result;
    {
      QTextOStream os_(&result);
      // Skip to indexth section
      const char **line_ = &a_list[0];
      for ( ; 0 != index; --index)
      {
   	for ( ; 0 != *line_; ++line_)
        {
          // do nothing
	}
	++line_;
      }
      for ( ; 0 != *line_; ++line_)
      {
        os_ << *line_;
      }
    }
    return result;
  }
  static QString file_to_string (const char *a_name)
  {
    QString result = "";
    {
      QFile input_file_(a_name);
      if (!input_file_.open (IO_ReadOnly))
      {
	const QString error_("Unable to open text file for reading: ");
        throw std::runtime_error((error_ + a_name).utf8().data());
      }
      while (!input_file_.atEnd ())
      {
        const int sz_ = 1024;
        char buf_[1024];
	const int read_sz_ (input_file_.readBlock(&buf_[0], sz_));
	if (0 > read_sz_)
	{
	  const QString error_("Reading of file failed for: ");
          throw std::runtime_error((error_ + a_name).utf8().data());
	}
        result.append(QString::fromUtf8(buf_, read_sz_));
      }
    }
    return result;
  }
};

// Set strings for the project name and version
const QString project_name_ (a_project.name ());
QString version_;
if (! a_project.property_value (item_traits::butter_version_name, version_))
{
  if (! a_project.property_value (item_traits::version_name, version_))
  {
    version_ = "1.0";
  }
}

// See if the user has defined some defaults for themselves for this style.
//
const char *const var_ = getenv (item_traits::app_data_env_var.utf8());
const QString app_data_var (var_ == NULL? "" : var_);
#ifdef DEBUG
butter::log::com.trace(butter::log::debug, ("Looking for env var: "
     + item_traits::app_data_env_var).utf8());
butter::log::com.trace(butter::log::debug, ("Value of var is: "
     + app_data_var).utf8());
#endif
if ( !app_data_var.isEmpty() && ! app_data_var.isNull ())
{
  QFileInfo app_data_info_((pathcmp (app_data_var)
    / item_traits::app_data_subdir_name
    / static_cast< derived* >(this)->section_name).path ());
#ifdef DEBUG
  butter::log::com.trace(butter::log::debug, ("Full path is: "
      + app_data_info_.absFilePath()).utf8());
#endif
  if (app_data_info_.exists ()
      && app_data_info_.isReadable()
      && app_data_info_.isDir ())
  {
#ifdef DEBUG
    butter::log::com.trace(butter::log::debug
        , "Path exists, is readable and is a directory");
#endif
    // For each file in directory create a UML document
    //
    QDir app_data_(app_data_info_.absFilePath());
    app_data_.setFilter(QDir::Files | QDir::NoSymLinks | QDir::Readable);
    for (unsigned int i_ = 0; i_ < app_data_.count (); ++i_)
    {
      if (a_base.find_uml_document (app_data_[i_]).isEmpty ())
      {
        a_base.create_uml_document (app_data_[i_])->set_Description (
          helper_::make_substitutions (
		helper_::file_to_string(app_data_.absFilePath(app_data_[i_]))
		     , project_name_, version_).utf8());
      }
    }
  }
}

// Now check to see if the default rules file(s) have been created
//
const QString &rules_name_ (static_cast< derived* >(this)->rules_name);
if (rules_name_.contains(" "))
{
  // Is a list of names?
  unsigned int i_ = 0;
  for (const_token_iterator e_, b_(rules_name_, ' '); b_ != e_; ++b_)
  {
    const QString name_(*b_);
    if (not name_.isEmpty ())
    {
      a_base.create_uml_document (name_)->set_Description (
        helper_::make_substitutions (
            helper_::list_to_string(&static_cast< derived* >(this)->default_rules[0], i_)
              , project_name_, version_).utf8());
      ++i_; // Only increment i for non-empty names!
    }
  }
}
else
{
  if (a_base.find_uml_document (rules_name_).isEmpty ())
  {
    a_base.create_uml_document (rules_name_)->set_Description (
       helper_::make_substitutions (
         helper_::list_to_string(
           &static_cast< derived* >(this)->default_rules[0], 0)
           , project_name_, version_).utf8());
  }
}

}


} // namespace butter
#endif
