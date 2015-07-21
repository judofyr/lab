#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_SIZES_H
#include FT_TRUETYPE_TABLES_H

#include <hb.h>

#include <ot/math.h>

#include <memory>

class Font {
  FT_Size ft_size;
  hb_font_t* hb_font;
public:

  Font(FT_Face, int pointSize);
  Font(const Font&) = delete;
  Font(Font&&) = default;
  ~Font();

  void activate();
  hb_font_t* hbFont();
  FT_Face ftFace();
  FT_Size ftSize();
};

class Face {
  FT_Face ft_face;

public:
  Face(FT_Face f) : ft_face(f) { }
  Face(const Face&);
  Face(Face&&);
  ~Face();

  Font createFont(int pointSize);
  std::shared_ptr<Font> createSharedFont(int pointSize);
  OtMATH::Table mathTable();
};

class FontManager {
  FT_Library lib;

public:
  FontManager();

  Face createFace(const char *filename);
};



