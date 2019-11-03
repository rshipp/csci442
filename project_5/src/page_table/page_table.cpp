/**
 * This file contains implementations for methods in the PageTable class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "page_table/page_table.h"

using namespace std;


size_t PageTable::get_present_page_count() const {
  size_t sum = 0;
  for (size_t i=0; i < rows.size(); i++) {
    if (rows[i].present) {
      sum++;
    }
  }
  return sum;
}


size_t PageTable::get_oldest_page() const {
  size_t page = -1, loaded = -1;
  for (size_t i = 0; i < rows.size(); i++) {
    if (rows[i].present && rows[i].loaded_at < loaded) {
      loaded = rows[i].loaded_at;
      page = i;
    }
  }
  return page;
}


size_t PageTable::get_least_recently_used_page() const {
  size_t page = -1, used = -1;
  for (size_t i = 0; i < rows.size(); i++) {
    if (rows[i].present && rows[i].last_accessed_at < used) {
      used = rows[i].last_accessed_at;
      page = i;
    }
  }
  return page;
}
