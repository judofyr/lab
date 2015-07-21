#include <ui/layout.h>
#include <ui/math.h>
#include <ui/text.h>

class Spring : public StackLayout {
  TextureAtom ta;
  bool loaded = false;
  public:
  Math::Renderer* math;

  virtual void layout() override {
    auto vg = canvas->vg;

    if (!loaded) {
      math = new Math::Renderer(*canvas->mathFace);
      auto t1 = math->text<Math::Display>("Hello");
      auto t2 = math->text<Math::Display>("123");
      auto frac = math->fraction<Math::Text>(t1, t2);
      ta.draw(*canvas, frac);
      loaded = true;
    }

    draw(ta);

    {
      auto r = takeSpace(500);
      nvgBeginPath(vg);
      nvgFillColor(vg, nvgRGBf(1.0, 0.0, 0.0));
      nvgRect(vg, r.x, r.y, r.w, r.h);
      nvgFill(vg);
    }

    takeSpace(100);

    {
      auto r = takeSpace(500);
      nvgBeginPath(vg);
      nvgFillColor(vg, nvgRGBf(0.0, 1.0, 0.0));
      nvgRect(vg, r.x, r.y, r.w, r.h);
      nvgFill(vg);
    }
  }
};


extern "C" Spring* create() {
  return new Spring;
}

extern "C" void destroy(Spring* ctx) {
  delete ctx;
}

