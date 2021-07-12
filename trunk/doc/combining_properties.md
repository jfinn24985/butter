# How do compilation properties combine

TopPackage :
  - includes : self
  - flags : self
  - ldflags : self
  - type (shared|static) : self

ExternalLibrary :
  - includes : self
  - flags : self
  - ldflags : self

SubPackage :
  - includes : self + parent
  - flags : self + parent
  - ldflags : self + parent
  - type (shared|static) : self

Target :
  - includes : self + parent + external
  - flags : self + parent + external
  - ldflags : self + parent + external + link-template(internal)
  - type (shared|static) : self

Source :
  - includes : self + target
  - flags : self + target
  - ldflags : self

# Processing of includes, flags and ldflags

* Each package has VARS that merge with parent packages
* Each external has VARS
* Each target has VARS that merge parent packages and 
  externals
* Each object has VARS that merge target

Conclusion:

TOPINC = X
TOPCXX = -x
TOPLNK = -a

SUBINC = $(TOPINC) + Y
SUBCXX = $(TOPCXX) + -y
SUBLNK = $(TOPLNK) + -b

TARINC = $(SUBINC) + Z + $(EXTINC)
TARCXX = $(SUBC++) + -z + $(EXTCXX)
TARLNK = $(SUBLNK) + -c + $(EXTLNK) + $(INTLNK) + $(OBJLNK)

OBJINC = $(TARINC) + M
OBJCXX = $(TARCXX) + -m
OBJLNK = -d

# Processing of library type

* Want closest value to override
* Standard property merge process appends further values
    * post-process to remove all but first list element
