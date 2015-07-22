#pragma once

#include "canvas.h"
#include <nanovg.h>

class Atom {
public:
  int width = 0;
  int height = 0;
  int descender = 0;

  virtual void draw(Canvas&, Position) = 0;
};

class PixelAtom {
public:
  int width = 0;
  int height = 0;
  int descender = 0;

  virtual void draw(PixelCanvas& data, Position) = 0;
};

class TextureAtom : public Atom {
  int image = 0;

public:
  void draw(Canvas& c, PixelAtom& pa);
  virtual void draw(Canvas &c, Position pos) override;
};

