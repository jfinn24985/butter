#ifndef BUTTER_GENERATOR_H
#define BUTTER_GENERATOR_H


#include "base_generator.h"

namespace butter {

// --
//Writer class for recursive template idiom.  This contains the implementation for
//the create_system () factory_ method. Derived class writers can use this class to handle
//the boiler-plate process of traversing the location and artifact sets - they implement
//descendent_link, external_target, initialise and local_target.
template<class derived>
class generator : public base_generator {
  public:
    //DRIVER METHOD
    //
    //Create a series of build-system artifacts based at a_base location. The
    //artifacts are inserted into a package at each location.
    void create_system();

    //Create the initial rules set.
    void rules_file();

};

} // namespace butter
#endif
