/**
 * This file contains implementations for methods in the Page class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "page/page.h"

using namespace std;


// Ensure PAGE_SIZE is initialized.
const size_t Page::PAGE_SIZE;


Page* Page::read_from_input(std::istream& in) {
  // TODO: implement me
  return nullptr;
}


size_t Page::size() const {
  // TODO: implement me
  return 0;
}


bool Page::is_valid_offset(size_t offset) const {
  // TODO: implement me
  return false;
}


char Page::get_byte_at_offset(size_t offset) {
  // TODO: implement me
  return 0;
}
