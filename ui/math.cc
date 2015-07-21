#include "math.h"

#define PXCONST(name) \
  font.unit2px(constants->name.value())

using namespace Math;

template<int S>
TextAtom Renderer::text(const char* text) {
  return TextAtom(fonts[S], text);
}

template TextAtom Renderer::text<Display>(const char* text);
template TextAtom Renderer::text<Text>(const char* text);
template TextAtom Renderer::text<Script>(const char* text);
template TextAtom Renderer::text<ScriptScript>(const char* text);

template<int S>
struct FractionMeasurement {
  int thickness;
  int axis_height;
  int shift_up;
  int shift_down;
  int num_gap_min;
  int den_gap_min;

  static_assert(S == Display || S == Text, "Fractions must be either Display or Text");

  FractionMeasurement(Renderer& r) {
    auto &font = *r.fonts[S];
    auto constants = r.table.header()->constants();
    thickness = PXCONST(fractionRuleThickness);
    axis_height = PXCONST(axisHeight);

    if (S == Display) {
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

template<int S>
Fraction Renderer::fraction(PixelAtom& num, PixelAtom& den) {
  auto m = FractionMeasurement<S>(*this);

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

  int rule_y = rule_top - num_top;
  int den_y = den_top - num_top;
  int baseline_y = -num_top;

  return Fraction(num, den, m.thickness, rule_y, den_y, baseline_y);
}

template Fraction Renderer::fraction<Text>(PixelAtom&, PixelAtom&);
template Fraction Renderer::fraction<Display>(PixelAtom&, PixelAtom&);

void Fraction::draw(PixelCanvas& c, Position pos) {
  // Draw line
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < thickness; y++) {
      auto p = c.pixel(pos, x, y+rule_y);
      c.setAlpha(p, 0xFF);
    }
  }

  // Center alignment
  int den_x;

  if (num.width < width) {
    int num_extra_x = (width - num.width)/2;
    pos.x += num_extra_x;
    den_x = -num_extra_x;
  } else {
    den_x = (width - den.width)/2;
  }

  num.draw(c, pos);

  pos.x += den_x;
  pos.y += den_y;

  den.draw(c, pos);
}

