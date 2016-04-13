/**
 * This file contains implementations for methods in the VirtualAddress class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "virtual_address/virtual_address.h"
#include <bitset>
#include <iostream>

using namespace std;


VirtualAddress VirtualAddress::from_string(int process_id, string address) {
  size_t addr = bitset<ADDRESS_BITS>(address).to_ulong();
  size_t page = addr >> OFFSET_BITS;
  size_t offset = addr - (page << OFFSET_BITS);

  return VirtualAddress(process_id, page, offset);
}


string VirtualAddress::to_string() const {
  int address = (page << OFFSET_BITS) + offset;
  return bitset<ADDRESS_BITS>(address).to_string();
}


ostream& operator <<(ostream& out, const VirtualAddress& address) {
  out
      << "PID " << address.process_id
      << " @ " << address.to_string()
      << " [page: " << address.page
      << "; offset: " << address.offset
      << "]";
  return out;
}
