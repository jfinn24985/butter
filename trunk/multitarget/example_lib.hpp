#ifndef _EXAMPLE_LIB_H
#define _EXAMPLE_LIB_H


#include <string>
using namespace std;

class example_lib {
  private:
    /**
     * The width of the line
     */
    int line_width;


  public:
    /**
     * Centre text a_value on line
     */
    string format(string a_value);

    inline const int get_line_width() const;

    inline void set_line_width(int value);

};
inline const int example_lib::get_line_width() const {
  return line_width;
}

inline void example_lib::set_line_width(int value) {
  line_width = value;
}

#endif
