#pragma once

#include "fonts.h"
#include "atom.h"
#include "text.h"
#include <ot/math.h>

#include <memory>
#include <algorithm>

namespace Math {
  enum Style {
    D, T, S, SS
  };

  template<int Style, int Cramped = 0>
  class MathBox : public PixelAtom {
  };

  class Renderer {
    public:
    Face face;
    std::shared_ptr<Font> fonts[4];

    OtMATH::Table table;

    Renderer(Face face_) : face(face_) {
      table = face.mathTable();

      auto constants = table.header()->constants();
      int font_size = 16;

      fonts[D] = face.createSharedFont(font_size);
      fonts[T] = fonts[D];
      fonts[S] = face.createSharedFont(font_size * constants->scriptPercentScaleDown() / 100);
      fonts[SS] = face.createSharedFont(font_size * constants->scriptScriptPercentScaleDown() / 100);
    }

  };

#define SAME(St, C) MathBox<St, C>

    // D -> T, T -> S, S -> SS, SS -> SS
#define DOWN(St, C) MathBox<std::min(St+1, SS+0), C>

    // D -> S, T -> S, S -> SS, SS -> SS
#define SCRIPT(St, C) MathBox<std::min(std::max(St+1, S+0), SS+0), C>

  template<int St, int C>
  class Text : public SAME(St, C) {
    TextAtom backend;
    public:

    Text(Renderer& r, const char* text) :
      backend(r.fonts[St], text) {

      this->width = backend.width;
      this->height = backend.height;
      this->descender = backend.descender;
    }

    virtual void draw(PixelCanvas& c, Position pos) override {
      backend.draw(c, pos);
    }
  };

  template<int St, int C>
  class Script : public SAME(St, C) {
    // TODO
    typedef SCRIPT(St, C)& Sup;
    typedef SCRIPT(St, 1)& Sub;
    SAME(St, C)& base;
    Sup sup;
    Sub sub;

    int sup_y;
    int sub_y;
  };

  template<int St, int C>
  class Fraction : public SAME(St, C) {
    typedef DOWN(St, C)& Num;
    typedef DOWN(St, 1)& Den;

    Num num;
    Den den;
    int thickness;

    int rule_y;
    int den_y;

    void setup(Renderer&);

    public:

    Fraction(Renderer& r, Num n, Den d) : num(n), den(d) {
      setup(r);
    }

    virtual void draw(PixelCanvas& c, Position pos) override;
  };

  template<int N, int St, int C=0>
  class HBox : public SAME(St, C) {
    SAME(St, C)* boxes[N];
    int idx = 0;
    int ascender = 0;

    public:
    void add(SAME(St, C)& other) {
      this->width += other.width;
      this->ascender = std::max(this->ascender, (other.height - other.descender));
      this->descender = std::max(this->descender, other.descender);
      this->height = this->ascender + this->descender;
      boxes[idx++] = &other;
    }

    virtual void draw(PixelCanvas& c, Position pos) override {
      for (int i = 0; i < N; i++) {
        auto &other = *boxes[i];
        auto box_pos = pos;
        box_pos.y += this->ascender - (other.height - other.descender);
        other.draw(c, box_pos);
        pos.x += other.width;
      }
    }
  };

  class Helper {
    Renderer& renderer;

    public:
    Helper(Renderer& r) : renderer(r) { }
    
    template<int St, int C=0>
    Text<St, C> text(const char *text) {
      return Text<St, C>(renderer, text);
    }

    template<int St, int C=0>
    Fraction<St, C> frac(DOWN(St, C)& num, DOWN(St, 1)& den) {
      return Fraction<St, C>(renderer, num, den);
    }
  };
}

