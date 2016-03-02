#include <string>
#include "process.h"

std::string process_type_str(ProcessType num) {
  std::string type;
  switch(num) {
      case SYSTEM:
          type = "SYSTEM";
          break;
      case INTERACTIVE:
          type = "INTERACTIVE";
          break;
      case NORMAL:
          type = "NORMAL";
          break;
      case BATCH:
          type = "BATCH";
          break;
  }
  return type;
}
