#include "canvas.h"

using Pixel = PixelCanvas::Pixel;

PixelCanvas::PixelCanvas(int w, int h) {
  width = w;
  height = h;
  buffer = std::make_unique<buffer_t>(w * h * 4);
}

Pixel PixelCanvas::pixel(Position pos) {
  if (pos.x < 0 || pos.x >= width)
    return { -1 };
  if (pos.y < 0 || pos.y >= height)
    return { -1 };
  return { pos.y*width + pos.x };
}

Pixel PixelCanvas::pixel(Position pos, int x, int y) {
  pos.x += x;
  pos.y += y;
  return pixel(pos);
}

void PixelCanvas::setRed(Pixel p, uint8_t val) {
  if (p.idx != -1)
    buffer[p.idx*4 + 0] = val;
}

void PixelCanvas::setGreen(Pixel p, uint8_t val) {
  if (p.idx != -1)
    buffer[p.idx*4 + 1] = val;
}

void PixelCanvas::setBlue(Pixel p, uint8_t val) {
  if (p.idx != -1)
    buffer[p.idx*4 + 2] = val;
}

void PixelCanvas::setAlpha(Pixel p, uint8_t val) {
  if (p.idx != -1)
    buffer[p.idx*4 + 3] = val;
}

