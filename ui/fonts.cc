#include "fonts.h"

#include <hb-ft.h>

#include <utility>
#include <cassert>

FontManager::FontManager()
{
  assert(!FT_Init_FreeType(&lib));
}

Face FontManager::createFace(const char *filename) {
  FT_Face ft_face;
  assert(!FT_New_Face(lib, filename, 0, &ft_face));
  return Face(ft_face);
}

Face::~Face() {
  FT_Done_Face(ft_face);
}

Font Face::createFont(int pointSize) {
  // TODO: Set DPI properly
  assert(!FT_Set_Char_Size(ft_face, pointSize << 6, 0, 72*2, 72*2));
  return Font(ft_face);
}

Font::Font(FT_Face f) {
  hb_font = hb_ft_font_create_referenced(f);
  auto &metrics = f->size->metrics;
  ascender = metrics.ascender;
  descender = metrics.descender;
  height = metrics.height;
}

Font::~Font() {
  hb_font_destroy(hb_font);
}

FT_Face Font::ftFace() {
  return hb_ft_font_get_face(hb_font);
}

