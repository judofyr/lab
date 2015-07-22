#include "math.h"

#define PXCONST(name) \
  font.unit2px(constants->name.value())

#define SPEC_ALL \
  SPEC(D, 0) SPEC(D, 1) \
  SPEC(T, 0) SPEC(T, 1) \
  SPEC(S, 0) SPEC(S, 1) \
  SPEC(SS, 0) SPEC(SS, 1)

using namespace Math;

template<int St>
struct FractionMeasurement {
  int thickness;
  int axis_height;
  int shift_up;
  int shift_down;
  int num_gap_min;
  int den_gap_min;

  FractionMeasurement(Renderer& r) {
    auto &font = *r.fonts[St];
    auto constants = r.table.header()->constants();
    thickness = PXCONST(fractionRuleThickness);
    axis_height = PXCONST(axisHeight);

    if (St == D) {
      shift_up = PXCONST(fractionNumeratorDisplayStyleShiftUp);
      shift_down = PXCONST(fractionDenominatorDisplayStyleShiftDown);
      num_gap_min = PXCONST(fractionNumDisplayStyleGapMin);
      den_gap_min = PXCONST(fractionDenomDisplayStyleGapMin);
    } else {
      shift_up = PXCONST(fractionNumeratorShiftUp);
      shift_down = PXCONST(fractionDenominatorShiftDown);
      num_gap_min = PXCONST(fractionNumeratorGapMin);
      den_gap_min = PXCONST(fractionDenominatorGapMin);
    }
  }
};

template<int St, int C>
void Fraction<St, C>::setup(Renderer& r) {
  auto m = FractionMeasurement<St>(r);

  int rule_top = -m.axis_height - m.thickness/2;
  int rule_bot = rule_top + m.thickness;

  int num_base = -m.shift_up;
  int num_bot = num_base + num.descender;
  if (rule_top - num_bot < m.num_gap_min) {
    num_bot = rule_top - m.num_gap_min;
  }
  int num_top = num_bot - num.height;

  int den_base = m.shift_down;
  int den_top = den_base - (den.height - den.descender);
  if (den_top - rule_bot < m.den_gap_min) {
    den_top = rule_bot + m.den_gap_min;
  }

  thickness = m.thickness;
  rule_y = rule_top - num_top;
  den_y = den_top - num_top;

  int baseline_y = 0 - num_top;

  this->width = std::max(num.width, den.width);
  this->height = den_y + den.height;
  this->descender = this->height - baseline_y;
}

template<int St, int C>
void Fraction<St, C>::draw(PixelCanvas& c, Position pos) {
  // Draw line
  for (int x = 0; x < this->width; x++) {
    for (int y = 0; y < thickness; y++) {
      auto p = c.pixel(pos, x, y+rule_y);
      c.setAlpha(p, 0xFF);
    }
  }

  // Center alignment
  int den_x;

  if (num.width < this->width) {
    int num_extra_x = (this->width - num.width)/2;
    pos.x += num_extra_x;
    den_x = -num_extra_x;
  } else {
    den_x = (this->width - den.width)/2;
  }

  num.draw(c, pos);

  pos.x += den_x;
  pos.y += den_y;

  den.draw(c, pos);
}


#define SPEC(St, C) \
  template void Fraction<St, C>::setup(Renderer&);\
  template void Fraction<St, C>::draw(PixelCanvas&, Position);

SPEC_ALL

