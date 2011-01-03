#include <qapplication.h>

#include "UmlCom.h"
#include "UmlItem.h"
#include "UmlPackage.h"
#include "UmlArtifact.h"
#include "butter/base_generator.h"
#include "butter/location.h"
#include "butter/style.h"
#include "butter/butter_constants.h"
#include "butter/config.h"

#include <qtextstream.h>
#include <qmessagebox.h>
#ifdef __unix__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <Windows.h>
#endif

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;

  QApplication a(argc, argv);

  if (UmlCom::connect(QString(argv[1]).toUInt()))
  {
    try
    {
      UmlCom::trace("<b><i>jamfile/makefile</i> generator</b> " + butter::butter_constants::BUTTER_VERSION + "<br>");
      if (DEBUG)
      {
        QString msg_;
        QTextOStream mos_ (&msg_);
#ifdef __unix__
        mos_ << "Hit Ok to continue. If you want to debug this application attach to process [pid="
	 << getpid () << "] before continuing.";
#else
#ifdef _WIN32
        mos_ << "Hit Ok to continue. If you want to debug this application attach to process [Id="
	 << GetCurrentProcessId () << "] before continuing.";
#else
        mos_ << "Hit Ok to continue. If you want to debug this application attach to butter process now.";
#endif
#endif
        QMessageBox msg_box_ (butter::butter_constants::BUTTER_VERSION, msg_, QMessageBox::Warning, QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton, QMessageBox::NoButton);
        msg_box_.exec ();
      }
      BUTTER_CHECK (NULL != UmlPackage::getProject (), "Error: no project defined!<br>");
      // Parse the project into a location tree
      std::auto_ptr< butter::location > base_ (butter::location::parse_project (*UmlPackage::getProject ()));
      // Generate the build-system files.
      std::auto_ptr< butter::base_generator > writer_ (butter::style::get_style ().create_writer ());
      writer_->create_system (*base_.get (), *UmlPackage::getProject ());
      // Write the build-system files.
      butter::location::write_documents (*base_.get ());
    }
    catch (const std::exception &a_err)
    {
      UmlCom::trace(a_err.what ());
    }
    catch (const char * a_what)
    {
      UmlCom::trace(a_what);
    }
    catch (...)
    {
      UmlCom::trace("Caught unknown exception.");
    }

    UmlCom::trace("<b><i>jamfile/makefile</i> generator</b> complete<br>");
    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }

  UmlCom::close();
  return 0;
}

#ifdef _WIN32
#if QT_VERSION < 300L
QStringData * QString::shared_null = 0;
QString QString::null = QString (0);
QChar QChar::null = 0;
#endif
#endif
