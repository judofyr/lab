#pragma once

#include "utils.h"
#include "atom.h"

class Layout {
public:
  virtual int draw(Canvas&, Position, int width) = 0;
  virtual ~Layout() = default;
};

class StackLayout : public Layout {
  Position start;
  int height;

  virtual void layout() = 0;

protected:
  Canvas* canvas;
  int width;

  void draw(Layout& l) {
    Position pos = start;
    pos.y += height;
    height += l.draw(*canvas, pos, width);
  }

  void draw(Atom& a) {
    Position pos = start;
    pos.y += height;
    a.draw(*canvas, pos);
    height += a.height;
  }

  Rect takeSpace(int h) {
    Rect res;
    res.x = start.x;
    res.y = start.y + height;
    res.w = width;
    res.h = h;
    height += h;
    return res;
  }

public:
  virtual int draw(Canvas& c, Position s, int w) override {
    canvas = &c;
    start = s;
    width = w;
    height = 0;
    layout();
    return height;
  }
};

