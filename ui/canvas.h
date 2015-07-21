#pragma once

#include "utils.h"
#include "fonts.h"

#include <nanovg.h>

#include <memory>

class Canvas {
public:
  NVGcontext* vg;
  std::shared_ptr<Font> paragraphFont;
  std::unique_ptr<Face> mathFace;

  Canvas(NVGcontext *v) : vg(v) { }

  void setParagraphFont(Font&& font) {
    paragraphFont = std::make_shared<Font>(std::move(font));
  }
};

class PixelCanvas {
  typedef unsigned char buffer_t[];

public:
  std::unique_ptr<buffer_t> buffer;
  int width, height;

  struct Pixel {
    int idx;
  };

  PixelCanvas(int w, int h);

  Pixel pixel(Position pos);
  Pixel pixel(Position pos, int x, int y);

  void setRed(Pixel p, uint8_t val);
  void setGreen(Pixel p, uint8_t val);
  void setBlue(Pixel p, uint8_t val);
  void setAlpha(Pixel p, uint8_t val);
};

