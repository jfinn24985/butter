
#include "UmlCom.h"
#include "UmlItem.h"

// the program is called with the socket port number in argument

int main(int argc, char ** argv)
{
  if (argc != 2)
    return 0;
  
  if (UmlCom::connect(QString(argv[1]).toUInt())) {
    try {
      // does something on the target, here suppose that a virtual
      // operation exist at UmlItem level (and probably sub_level !)
      UmlCom::targetItem()->???();

      // must be called to cleanly inform that all is done
      UmlCom::bye(0); // 0 means no error

      UmlCom::close();
    }
    catch (...) {
    }
  }
  
  return 0;
}
