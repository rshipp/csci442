#pragma once
#include <stddef.h>

/**
 * The type of the burst.
 */
enum BurstType {
  CPU,
  IO
};

/**
* Simple struct representing a CPU or IO burst.
*/
struct Burst {
  /**
   * The type of burst.
   */
  BurstType type;

  /**
   * The length of the burst.
   */
  size_t length;

  /**
   * Creates a burst of the given type and length.
   */
  Burst(BurstType type, size_t length) : type(type), length(length) {}
};
