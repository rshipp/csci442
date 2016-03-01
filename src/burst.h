#pragma once

/**
* Simple struct representing a CPU or IO burst.
*/
struct Burst {
  /**
   * The type of the burst.
   */
  enum Type {
    CPU,
    IO
  };

  /**
   * The type of burst.
   */
  Type type;

  /**
   * The length of the burst.
   */
  size_t length;

  /**
   * Creates a burst of the given type and length.
   */
  Burst(Type type, size_t length) : type(type), length(length) {}
};
