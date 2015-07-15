#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_SIZES_H
#include FT_TRUETYPE_TABLES_H

#include <hb.h>

#include <ot/math.h>

class Font {
public:
  hb_font_t* hb_font;
  int ascender, descender, height;

  Font(FT_Face);
  FT_Face ftFace();
  Font(const Font&) = delete;
  Font(Font&&) = default;
  ~Font();
};

class Face {
  FT_Face ft_face;

public:
  Face(FT_Face f) : ft_face(f) { }
  Face(const Face&) = delete;
  Face(Face&&) = default;
  ~Face();

  Font createFont(int pointSize);
  OtMATH::Table mathTable();
};

class FontManager {
  FT_Library lib;

public:
  FontManager();

  Face createFace(const char *filename);
};



