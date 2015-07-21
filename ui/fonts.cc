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

Face::Face(const Face& other) {
  ft_face = other.ft_face;
  FT_Reference_Face(ft_face);
}

Face::Face(Face&& other) {
  ft_face = other.ft_face;
  other.ft_face = nullptr;
}

Face::~Face() {
  if (ft_face)
    FT_Done_Face(ft_face);
}

Font Face::createFont(int pointSize) {
  return Font(ft_face, pointSize);
}

std::shared_ptr<Font> Face::createSharedFont(int pointSize) {
  return std::make_shared<Font>(ft_face, pointSize);
}

OtMATH::Table Face::mathTable() {
  FT_ULong length = 0;
  FT_Tag tag = FT_MAKE_TAG('M','A','T','H');

  {
    auto err = FT_Load_Sfnt_Table(ft_face, tag, 0, NULL, &length);
    if (err) return OtMATH::Table();
  }

  OtMATH::Table table(length);

  {
    auto err = FT_Load_Sfnt_Table(ft_face, tag, 0, table.buffer(), &length);
    if (err) return OtMATH::Table();
  }

  return table;
}

Font::Font(FT_Face f, int pointSize) {
  assert(!FT_New_Size(f, &ft_size));
  assert(!FT_Activate_Size(ft_size));
  // TODO: Set DPI properly
  assert(!FT_Set_Char_Size(f, pointSize << 6, 0, 72*2, 72*2));
  hb_font = hb_ft_font_create_referenced(f);
}

Font::~Font() {
  FT_Done_Size(ft_size);
  hb_font_destroy(hb_font);
}

hb_font_t* Font::hbFont() {
  return hb_font;
}

FT_Face Font::ftFace() {
  assert(!FT_Activate_Size(ft_size));
  return hb_ft_font_get_face(hb_font);
}

FT_Size Font::ftSize() {
  return ft_size;
}


