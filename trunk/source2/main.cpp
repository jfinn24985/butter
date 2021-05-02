// the program is called with the socket port number in argument

int main( int argc, char ** argv )
{
  unsigned int port = 0; // Port for connection to bouml.
  // User-interaction variable
  //    0 = ask user
  //    1 = assume all answers are "Ok"
  //    2 = assume all answers are "Abort/Cancel"
  unsigned int interaction = 0;
  QApplication a( argc, argv );
  if ( argc < 2 )
  {
    return -1;
  }

  if( argc == 2 )
  {
    port = QString( argv[1] ).toUInt();
  }
  else
  {
    for( int c_ = 1; c_ != argc; ++c_ )
    {
      if( QString( argv[c_] ) == "-test:ok" )
      {
        interaction = 1;
      }
      else if ( QString( argv[c_] ) == "-test:cancel" )
      {
         interaction = 2;
      }
      else
      {
        port = QString( argv[c_] ).toUInt();
      }
    }
  }

  if ( UmlCom::connect( port ) )
  {
    try
    {
      UmlCom::trace( "<b><i>jamfile/makefile</i> generator</b> " + butter::constants::BUTTER_VERSION + "<br>" );
      if ( DEBUG and 0 == interaction )
      {
        QString msg_;
        QTextOStream mos_( &msg_ );
#ifdef __unix__
        mos_ << "Hit Ok to continue. If you want to debug this application attach to process [pid="
          << getpid() << "] before continuing.";
#else
#ifdef _WIN32
        mos_ << "Hit Ok to continue. If you want to debug this application attach to process [Id="
          << GetCurrentProcessId() << "] before continuing.";
#else
        mos_ << "Hit Ok to continue. If you want to debug this application attach to butter process now.";
#endif
#endif
        QMessageBox msg_box_( butter::constants::BUTTER_VERSION, msg_, QMessageBox::Warning, QMessageBox::Ok | QMessageBox::Default, QMessageBox::NoButton, QMessageBox::NoButton );
        msg_box_.exec();
      }
      BUTTER_CHECK( NULL != UmlPackage::getProject (), "Error: no project defined!<br>" );
      // Parse the project into a location tree
      std::unique_ptr< butter::location > base_( butter::location::parse_project( *UmlPackage::getProject(), interaction ) );
      // Generate the build-system files.
      std::unique_ptr< butter::base_generator > writer_( butter::style::get_style().create_writer() );
      writer_->create_system( *base_.get(), *UmlPackage::getProject() );
      // Write the build-system files.
      butter::location::write_documents( *base_.get() );
    }
    catch ( const std::exception & a_err )
    {
      UmlCom::trace( a_err.what () );
    }
    catch ( const char * a_what )
    {
      UmlCom::trace( a_what );
    }
    catch ( ... )
    {
      UmlCom::trace( "Caught unknown exception." );
    }

    UmlCom::trace( "<b><i>jamfile/makefile</i> generator</b> complete<br>" );
    // must be called to cleanly inform that all is done
    UmlCom::bye();
  }

  UmlCom::close();
  return 0;
}
