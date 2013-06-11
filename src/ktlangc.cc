/*************************************************************************************************
 * C language binding
 *                                                               Copyright (C) 2009-2012 FAL Labs
 * This file is part of Kyoto Cabinet.
 * This program is free software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation, either version
 * 3 of the License, or any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************************************/


#include <ktremotedb.h>
#include "ktlangc.h"

using namespace kyototycoon;

extern "C" {
#include "postgres.h"

/**
 * Create a database object.
 */
KTDB* ktdbnew(void) {
  _assert_(true);
  return (KTDB*)new RemoteDB;
}


/**
 * Destroy a database object.
 */
void ktdbdel(KTDB* db) {
  _assert_(db);
  RemoteDB* pdb = (RemoteDB*)db;
  delete pdb;
}


/**
 * Open a database file.
 */
int32_t ktdbopen(KTDB* db, const char* host, int32_t port, double timeout) {
  _assert_(db && host && port && timeout);
  RemoteDB* pdb = (RemoteDB*)db;
  return pdb->open(host, port, timeout);
}


/**
 * Close the database file.
 */
int32_t ktdbclose(KTDB* db) {
  _assert_(db);
  RemoteDB* pdb = (RemoteDB*)db;
  return pdb->close();
}

int64_t ktdbcount(KTDB* db) {
  _assert_(db);
  RemoteDB* pdb = (RemoteDB*)db;
  return pdb->count();
}

KTCUR* get_cursor(KTDB* db) {
  _assert_(db);
  RemoteDB* pdb = (RemoteDB*)db;
  RemoteDB::Cursor *cur = pdb->cursor();
  cur->jump();
  return (KTCUR*) cur;
}

void ktcurdel(KTCUR* cur) {
  _assert_(cur);
  RemoteDB::Cursor *rcur = (RemoteDB::Cursor *) cur;
  delete rcur;
}


bool next(KTDB* db, KTCUR* cur, char **key, char **value)
{
  std::string skey;
  std::string sval;

  RemoteDB::Cursor *rcur = (RemoteDB::Cursor *) cur;
  bool res = rcur->get(&skey, &sval, NULL, true);
  if(!res)
    return false;

  *key = (char *) palloc(sizeof(char)*(skey.length()+1));
  *value = (char *) palloc(sizeof(char)*(sval.length()+1));

  std::strcpy(*key, skey.c_str());
  std::strcpy(*value, sval.c_str());
  return true;
}

}

// END OF FILE