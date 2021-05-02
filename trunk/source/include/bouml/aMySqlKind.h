#ifndef _AMYSQLKIND_H
#define _AMYSQLKIND_H

#ifdef WITHMYSQL


enum aMySqlKind {
  aColumn,
  aPrimaryKey,
  anUniqueKey,
  aKey,
  aFulltextKey,
  aSpatialKey

};

#endif

#endif
