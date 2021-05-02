#ifndef _AMYSQLREFOPTION_H
#define _AMYSQLREFOPTION_H

#ifdef WITHMYSQL


enum aMySqlRefOption {
  anUnspecifiedRefOption,
  doRestrict,
  doCascade,
  doSetNull,
  doNoAction

};

#endif

#endif
