#include "text.h"

#include <cassert>
#include <algorithm>

static auto hb_en = hb_language_from_string("en", 2);

TextAtom::TextAtom(std::shared_ptr<Font> f, const char *t) : font(f) {
  font->activate();

  buf = hb_buffer_create();
  hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
  hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
  hb_buffer_set_language(buf, hb_en);

  hb_buffer_add_utf8(buf, t, -1, 0, -1);
  hb_shape(font->hbFont(), buf, nullptr, 0);

  unsigned int glyph_count;
  hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);
  hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);

  int y_bearing = 0;
  int w = 0;
  int h = 0;

  hb_glyph_extents_t extents;


  for (int i = 0; i < glyph_count; i++) {
    hb_font_get_glyph_extents(font->hbFont(), glyph_info[i].codepoint, &extents);
    w += glyph_pos[i].x_advance;
    h = std::max(h, -extents.height);
    y_bearing = std::max(y_bearing, extents.y_bearing);
  }

  descender = (h - y_bearing) >> 6;
  height = h >> 6;
  width = w >> 6;
}

TextAtom::~TextAtom() {
  hb_buffer_destroy(buf);
}

void TextAtom::draw(PixelCanvas& c, Position pos) {
  font->activate();

  unsigned int glyph_count;
  hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
  hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);

  auto face = font->ftFace();

  // Measured in 26.6 pixels.
  int pen_x = 0;
  int pen_y = (height - descender) << 6;

  for (int i = 0; i < glyph_count; i++) {
    auto &gi = glyph_info[i];
    auto &gp = glyph_pos[i];

    // Load the glyph-bitmap
    assert(!FT_Load_Glyph(face, gi.codepoint, FT_LOAD_RENDER));
    auto &bm = face->glyph->bitmap;
    for (int y = 0; y < bm.rows; y++) {
      for (int x = 0; x < bm.width; x++) {
        int pos_y = (pen_y >> 6) + y - face->glyph->bitmap_top;
        int pos_x = (pen_x >> 6) + x + face->glyph->bitmap_left;
        auto p = c.pixel(pos, pos_x, pos_y);
        c.setAlpha(p, bm.buffer[y*bm.width + x]);
      }
    }

    pen_x += gp.x_advance;
  }
}

