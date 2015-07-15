#include "atom.h"

void TextureAtom::draw(Canvas& c, PixelAtom& pa) {
  if (image) {
    // TODO: optimize when same size?
    nvgDeleteImage(c.vg, image);
    image = 0;
  }

  width = pa.width;
  height = pa.height;

  auto pc = PixelCanvas(width, height);
  pa.draw(pc, {0,0});

  image = nvgCreateImageRGBA(c.vg, width, height, 0, pc.buffer.get());
  // TODO: Clear image on dealloc
}

void TextureAtom::draw(Canvas &c, Position pos) {
  if (!image)
    return;

  auto imgPaint = nvgImagePattern(c.vg, pos.x, pos.y, width, height, 0, image, 1.0);

  nvgBeginPath(c.vg);
  nvgRect(c.vg, pos.x, pos.y, width, height);
  nvgFillPaint(c.vg, imgPaint);
  nvgFill(c.vg);
}

