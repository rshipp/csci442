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
  char* buffer = new char[PAGE_SIZE];
  in.read(buffer, PAGE_SIZE);
  if (in.gcount() > 0) {
    vector<char> bytes(buffer, buffer + in.gcount());
    return new Page(bytes);
  } else {
    return nullptr;
  }
}


size_t Page::size() const {
  return bytes.size();
}


bool Page::is_valid_offset(size_t offset) const {
  if (offset < bytes.size()) {
    return true;
  }
  return false;
}


char Page::get_byte_at_offset(size_t offset) {
  return bytes[offset];
}
