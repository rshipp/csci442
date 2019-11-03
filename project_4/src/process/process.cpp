/**
 * This file contains implementations for methods in the Process class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "process/process.h"

using namespace std;


Process* Process::read_from_input(std::istream& in) {
  Page* page;
  size_t num_bytes = 0;
  vector<Page*> pages;
  while ((page = Page::read_from_input(in))) {
    pages.push_back(page);
    num_bytes += in.gcount();
  }
  return new Process(num_bytes, pages);
}


size_t Process::size() const {
  return num_bytes;
}


bool Process::is_valid_page(size_t index) const {
  return pages.size() > index;
}


size_t Process::get_rss() const {
  int sum = 0;
  for (size_t i = 0; i < page_table.rows.size(); i++) {
    if (page_table.rows[i].present) {
      sum++;
    }
  }
  return sum;
}


double Process::get_fault_percent() const {
  if (memory_accesses < 1) return 0.0;
  return 100 * page_faults/memory_accesses;
}
