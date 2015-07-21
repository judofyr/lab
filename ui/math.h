#pragma once

#include "fonts.h"
#include "atom.h"
#include "text.h"
#include <ot/math.h>

#include <memory>
#include <algorithm>

namespace Math {
  enum Style {
    Display, Text, Script, ScriptScript
  };

  class Fraction;

  class Renderer {
    public:
    Face face;
    std::shared_ptr<Font> fonts[4];

    OtMATH::Table table;

    Renderer(Face face_) : face(face_) {
      table = face.mathTable();

      auto constants = table.header()->constants();
      int font_size = 16;

      fonts[Display] = face.createSharedFont(font_size);
      fonts[Text] = fonts[Display];
      fonts[Script] = face.createSharedFont(font_size * constants->scriptPercentScaleDown() / 100);
      fonts[ScriptScript] = face.createSharedFont(font_size * constants->scriptScriptPercentScaleDown() / 100);
    }

    template<int S>
    TextAtom text(const char*);

    template<int S>
    Fraction fraction(PixelAtom& num, PixelAtom& den);
  };

  class Fraction : public PixelAtom {
    PixelAtom& num;
    PixelAtom& den;
    int thickness;

    int rule_y;
    int den_y;

    public:

    Fraction(PixelAtom& num_, PixelAtom& den_, int thickness_, int rule_y_, int den_y_, int baseline_y_) :
      num(num_), den(den_), thickness(thickness_), rule_y(rule_y_), den_y(den_y_)
    {
      width = std::max(num.width, den.width);
      height = den_y + den.height;
      descender = height - baseline_y_;
    }

    virtual void draw(PixelCanvas& c, Position pos) override;
  };
}

