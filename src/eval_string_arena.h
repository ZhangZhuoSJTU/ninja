#ifndef NINJA_EVAL_STRING_ARENA_H_
#define NINJA_EVAL_STRING_ARENA_H_

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "string_piece.h"

#ifndef NINJA_TEXT_ARENA
#define NINJA_TEXT_ARENA 0
#endif

class EvalStringArena {
 public:
  struct Ref {
    size_t length = 0;
#if NINJA_UNSAFE_TEXT_ARENA
    const char* pointer = nullptr;
#else
    size_t offset = 0;
#endif
  };

  EvalStringArena() { bytes_.reserve(4096); }

  Ref Store(StringPiece text) {
    const size_t offset = bytes_.size();
    if (text.size())
      bytes_.append(text.str_, text.size());
    Ref ref;
    ref.length = text.size();
#if NINJA_UNSAFE_TEXT_ARENA
    ref.pointer = bytes_.data() + offset;
#else
    ref.offset = offset;
#endif
    return ref;
  }

  std::string Read(const Ref& ref) const {
    if (!ref.length)
      return std::string();
#if NINJA_UNSAFE_TEXT_ARENA
    return std::string(ref.pointer, ref.length);
#else
    if (ref.offset > bytes_.size() ||
        ref.length > bytes_.size() - ref.offset) {
      std::fputs("invalid EvalString arena reference\n", stderr);
      std::abort();
    }
    return bytes_.substr(ref.offset, ref.length);
#endif
  }

 private:
  std::string bytes_;
};

#endif
