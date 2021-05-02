
#include "UmlCom.h"
#include "bouml/UmlBaseUseCaseReference.h"
#include "bouml/UmlUseCase.h"
#include "bouml/UmlFragmentCompartment.h"
#include "bouml/UmlSubject.h"
#include "bouml/UmlFragment.h"

void UmlBaseUseCaseReference::read_(const Q3PtrVector<UmlFragment> & fragments, const Q3PtrVector<UmlSubject> & subjects) {
  _use_case = (UmlUseCase *) UmlBaseItem::read_();
  _x = (int) UmlCom::read_unsigned();
  _y = (int) UmlCom::read_unsigned();
  _w = (int) UmlCom::read_unsigned();
  _h = (int) UmlCom::read_unsigned();
  _fragment = UmlBaseFragment::get_container_(_x, _y, _w, _h, fragments);
  _subject = UmlBaseSubject::get_container_(_x, _y, _w, _h, subjects);
}

