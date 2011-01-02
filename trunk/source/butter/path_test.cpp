// #include <QDir.h>
#include <butter/utility.h>
#include <iostream>

void do_path_test (QString const& a1, QString const& a2, QString const& a3, QString const& a4)
{
try {
#ifdef _MSC_VER
  int i;
#define MSC_NO_INT
#else
#define MSC_NO_INT int
#endif
  butter::pathcmp p1, p2;
  std::cerr << "Created empty path\n";
  p1 = a1;
  std::cerr << "Path1 \"" << p1.path () << "\" assigned from C string \"" << a1 << "\"\n";
  std::cerr << "Has " << p1.depth () << " lvls\n";
  for (MSC_NO_INT i = 0; i < p1.depth (); ++i)
  {
    std::cerr << "path[" << i + 1 << "] = \"" << p1.leaf_at (i) << "\"\n";
  }
  p2 = a2;
  std::cerr << "Path2 \"" << p2.path () << "\" assigned from C string \"" << a2 << "\"\n";
  std::cerr << "Has " << p2.depth () << " lvls\n";
  for (MSC_NO_INT i = 0; i < p2.depth (); ++i)
  {
    std::cerr << "path[" << i + 1 << "] = \"" << p2.leaf_at (i) << "\"\n";
  }
  butter::pathcmp p3 (a3);
  std::cerr << "Path3 \"" << p3.path () << "\" created from C string \"" << a3 << "\"\n";
  std::cerr << "Has " << p3.depth () << " lvls\n";
  for (MSC_NO_INT i = 0; i < p3.depth (); ++i)
  {
    std::cerr << "path[" << i + 1 << "] = \"" << p3.leaf_at (i) << "\"\n";
  }
  if (p1.equality (p2))
  {
    std::cerr << "Correctly found that path 1 and 2 are equal.\n";
  }
  else
  {
    std::cerr << "Incorrectly found that path 1 and 2 are not equal.\n";
  }
  p2 = a4;
  std::cerr << "Path2 \"" << p2.path () << "\" reassigned from C string \"" << a4 << "\"\n";
  std::cerr << "Has " << p2.depth () << " lvls\n";
  for (MSC_NO_INT i = 0; i < p2.depth (); ++i)
  {
    std::cerr << "path[" << i + 1 << "] = \"" << p2.leaf_at (i) << "\"\n";
  }
  if (p1.equality (p2))
  {
    std::cerr << "Incorrectly found that path 1 and 2 are equal.\n";
  }
  else
  {
    std::cerr << "Correctly found that path 1 and 2 are not equal.\n";
  }
  if (p3.has_subpath (p1))
  {
    std::cerr << "Correctly found that path 1 is a subpath of 3.\n";
  }
  else
  {
    std::cerr << "Incorrectly found that path 1 is not a subpath of 3.\n";
  }
  if (p3.has_subpath (p2))
  {
    std::cerr << "Incorrectly found that path 2 is a subpath of 3.\n";
  }
  else
  {
    std::cerr << "Correctly found that path 2 is not a subpath of 3.\n";
  }
  std::cerr << "Common path of 1 and 3 (should be == 3) is: " << p1.create_common (p3) << "\n";
  std::cerr << "Common path of 3 and 1 (should be == 3) is: " << p3.create_common (p1) << "\n";
  std::cerr << "Common path of 2 and 3 is: " << p2.create_common (p3) << "\n";
  std::cerr << "Common path of 3 and 2 is: " << p3.create_common (p2) << "\n";
  std::cerr << "Relative path 3 to 2 is: " << p3.create_relative (p2) << "\n";
  std::cerr << "Relative path 2 to 3 is: " << p2.create_relative (p3) << "\n";
  std::cerr << "Relative path 3 to 1 is: " << p3.create_relative (p1) << "\n";
  std::cerr << "Relative path 1 to 3 is: " << p1.create_relative (p3) << "\n";
}
catch (const std::exception &err)
{
  std::cerr << "Caught std::exception: " << err.what () << "\n";
}  
catch (char const *const err)
{
  std::cerr << "Caught C string exception " << err << "\n";
}
catch (...)
{
  std::cerr << "Caught unknown exception\n";
}  
}




int main (int argc, char** argv)
{
  do_path_test ("/home/is/where/heart/is"
	, "/home/is/where/heart/is/"
	, "/home/is/where"
	, "/home/is/hwere/heart/is");
  do_path_test ("C:\\home\\is\\where\\heart\\is"
	, "C:\\home\\is\\where\\heart\\is\\"
	, "C:\\home\\is\\where"
	, "D:\\home\\is\\where\\heart\\is");
  return 0;
}