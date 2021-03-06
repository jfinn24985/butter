
#include "UmlCom.h"
#include "bouml/UmlBaseFragmentCompartment.h"
#include "bouml/UmlFragment.h"
#include "bouml/UmlBaseFragment.h"

void UmlBaseFragmentCompartment::add_contained_(UmlFragment * x) {
  _contained.resize(_contained.size() + 1);
  _contained.insert(_contained.size() - 1, x);
}

void UmlBaseFragmentCompartment::add_text_(QByteArray x) {
  _texts.resize(_texts.size() + 1);
  _texts.insert(_texts.size() - 1, strdup(x));
}

/**
 * internal, do NOT use it
 */
void UmlBaseFragmentCompartment::add_cont_(QByteArray s, int cy) {
    if (cy < _fragment->vcenter_(_rank))
      _start_continuation = s;
    else
      _end_continuation = s;
}

bool UmlBaseFragmentCompartment::smaller(const UmlBaseFragmentCompartment * x) const {
  return ((_fragment->w() < x->_fragment->w()) &&
          (_fragment->h() < x->_fragment->h()));
}

/**
 *  internal, don't call it
 */
void UmlBaseFragmentCompartment::read_(UmlBaseFragment * fragment, int rank) {
    _fragment = (UmlFragment *) fragment;
    _rank = rank;
    _y = UmlCom::read_unsigned();
}

 UmlBaseFragmentCompartment::~UmlBaseFragmentCompartment() {
}

