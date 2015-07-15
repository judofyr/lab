#pragma once

#include "atom.h"
#include "fonts.h"

#include <memory>

class TextAtom : public PixelAtom {
  std::shared_ptr<Font> font;
  hb_buffer_t* buf;

public:
  TextAtom(std::shared_ptr<Font> f, const char *t);
  ~TextAtom();
  virtual void draw(PixelCanvas& c, Position pos);
};

