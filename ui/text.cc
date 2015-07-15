#include "text.h"

#include <cassert>

TextAtom::TextAtom(std::shared_ptr<Font> f, const char *t) : font(f) {
  buf = hb_buffer_create();
  hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
  hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
  hb_buffer_set_language(buf, hb_language_from_string("en", 2));

  hb_buffer_add_utf8(buf, t, -1, 0, -1);
  hb_shape(font->hbFont(), buf, nullptr, 0);

  auto &metrics = font->ftSize()->metrics;
  height = (metrics.ascender - metrics.descender) >> 6;
  width = 0;

  int w = 0;
  unsigned int glyph_count;
  hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);
  for (int i = 0; i < glyph_count; i++) {
    w += glyph_pos[i].x_advance;
  }

  width = w >> 6;
}

TextAtom::~TextAtom() {
  hb_buffer_destroy(buf);
}

void TextAtom::draw(PixelCanvas& c, Position pos) {
  unsigned int glyph_count;
  hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
  hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);

  auto face = font->ftFace();
  auto &metrics = font->ftSize()->metrics;

  // Measured in 26.6 pixels.
  int pen_x = 0;
  int pen_y = metrics.ascender;

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

