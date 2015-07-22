#include <ui/layout.h>
#include <ui/math.h>
#include <ui/text.h>

using namespace Math;

class Spring : public StackLayout {
  TextureAtom ta;
  bool loaded = false;
  public:
  Math::Renderer* math;

  virtual void layout() override {
    auto vg = canvas->vg;

    takeSpace(100);

    if (!loaded) {
      math = new Math::Renderer(*canvas->mathFace);
      auto h = Math::Helper(*math);
      auto t1 = h.text<T>("1");
      auto t2 = h.text<T,1>("2");
      auto frac = h.frac<D>(t1, t2);
      auto t3 = h.text<D>(" + 5 - 4");
      HBox<2, D> box;
      box.add(frac);
      box.add(t3);
      ta.draw(*canvas, box);
      loaded = true;
    }

    draw(ta);

    takeSpace(100);

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

