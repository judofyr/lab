#pragma once

#include <arpa/inet.h>

#include <memory>

namespace OtMATH
{
  template<typename T>
  class Value {
  protected:
    T raw;
  public:
    inline const T operator()() const { return raw; };
  };

  typedef Value<uint8_t> Byte;
  typedef Value<int8_t> Char;
  typedef Value<uint16_t> UShort;
  typedef Value<int16_t> Short;
  typedef Value<uint32_t> ULong;
  typedef Value<int32_t> Long;
  typedef UShort GlyphID;
  typedef UShort Offset;

  template<> inline const uint16_t UShort::operator()() const { return ntohs(raw); };
  template<> inline const uint32_t ULong::operator()() const { return ntohl(raw); };
  template<> inline const int16_t Short::operator()() const { return (int16_t)ntohs((uint16_t)raw); };
  template<> inline const int32_t Long::operator()() const { return (int32_t)ntohl((uint32_t)raw); };

  class Fixed : Value<uint32_t> {
  public:
    uint16_t integral() const { return ntohs(raw & 0xFFFF); }
    uint16_t fractal() const { return ntohs(raw >> 16); }
  };

  struct TableHeader;
  struct Constants;
  struct GlyphInfo;
  struct Variants;

  struct ItalicsCorrectionInfo;
  struct TopAccentAttachment;
  struct KernInfo;
  struct KernInfoRecord;
  struct Kern;

  struct GlyphConstruction;
  struct GlyphVariant;
  struct GlyphAssembly;
  struct GlyphPart;

  struct Table {
    std::unique_ptr<unsigned char[]> buf;
  public:

    Table() = default;

    Table(uint64_t size) {
      buf = std::make_unique<unsigned char[]>(size);
    }

    unsigned char* buffer() {
      return buf.get();
    }

    TableHeader* header() {
      return (TableHeader*)buf.get();
    }
  };

#define OTMATH_OFFSET_(klass, name, field) \
  Offset field; \
  inline klass* name() { \
    return (klass*)((char*)this + field()); \
  }
#define OTMATH_OFFSET(klass, name) \
  OTMATH_OFFSET_(klass, name, name ## Offset)

  struct Coverage {
    UShort format;

    struct Coverage1 {
      UShort count;
      GlyphID glyphs[1];
    };

    struct Coverage2 {
      UShort count;

      struct RangeRecord {
        GlyphID start;
        GlyphID end;
        UShort startIdx;
      };

      RangeRecord records[1];
    };

    union {
      Coverage1 one;
      Coverage2 two;
    };
  };

  struct ValueRecord {
    Short value;
    UShort deviceTable;
  };

  struct TableHeader {
    Fixed version;

    OTMATH_OFFSET(Constants, constants);
    OTMATH_OFFSET(GlyphInfo, glyphInfo);
    OTMATH_OFFSET(Variants, variants);
  };

  struct Constants {
    Short scriptPercentScaleDown;
    Short scriptScriptPercentScaleDown;
    Short delimitedSubFormulaMinHeight;
    Short displayOperatorMinHeight;
    ValueRecord mathLeading;
    ValueRecord axisHeight;
    ValueRecord accentBaseHeight;
    ValueRecord flattenedAccentBaseHeight;
    ValueRecord subscriptShiftDown;
    ValueRecord subscriptTopMax;
    ValueRecord subscriptBaselineDropMin;
    ValueRecord superscriptShiftUp;
    ValueRecord superscriptShiftUpCramped;
    ValueRecord superscriptBottomMin;
    ValueRecord superscriptBaselineDropMax;
    ValueRecord subSuperscriptGapMin;
    ValueRecord superscriptBottomMaxWithSubscript;
    ValueRecord spaceAfterScript;
    ValueRecord upperLimitGapMin;
    ValueRecord upperLimitBaselineRiseMin;
    ValueRecord lowerLimitGapMin;
    ValueRecord lowerLimitBaselineDropMin;
    ValueRecord stackTopShiftUp;
    ValueRecord stackTopDisplayStyleShiftUp;
    ValueRecord stackBottomShiftDown;
    ValueRecord stackBottomDisplayStyleShiftDown;
    ValueRecord stackGapMin;
    ValueRecord stackDisplayStyleGapMin;
    ValueRecord stretchStackTopShiftUp;
    ValueRecord stretchStackBottomShiftDown;
    ValueRecord stretchStackGapAboveMin;
    ValueRecord stretchStackGapBelowMin;
    ValueRecord fractionNumeratorShiftUp;
    ValueRecord fractionNumeratorDisplayStyleShiftUp;
    ValueRecord fractionDenominatorShiftDown;
    ValueRecord fractionDenominatorDisplayStyleShiftDown;
    ValueRecord fractionNumeratorGapMin;
    ValueRecord fractionNumDisplayStyleGapMin;
    ValueRecord fractionRuleThickness;
    ValueRecord fractionDenominatorGapMin;
    ValueRecord fractionDenomDisplayStyleGapMin;
    ValueRecord skewedFractionHorizontalGap;
    ValueRecord skewedFractionVerticalGap;
    ValueRecord overbarVerticalGap;
    ValueRecord overbarRuleThickness;
    ValueRecord overbarExtraAscender;
    ValueRecord underbarVerticalGap;
    ValueRecord underbarRuleThickness;
    ValueRecord underbarExtraDescender;
    ValueRecord radicalVerticalGap;
    ValueRecord radicalDisplayStyleVerticalGap;
    ValueRecord radicalRuleThickness;
    ValueRecord radicalExtraAscender;
    ValueRecord radicalKernBeforeDegree;
    ValueRecord radicalKernAfterDegree;
    Short radicalDegreeBottomRaisePercent;
  };

  struct GlyphInfo {
    OTMATH_OFFSET(ItalicsCorrectionInfo, italicsCorrectionInfo);
    OTMATH_OFFSET(TopAccentAttachment, topAccentAttachment);
    OTMATH_OFFSET(Coverage, extendedShapeCoverage);
    OTMATH_OFFSET(KernInfo, kernInfo);
  };

  struct ItalicsCorrectionInfo {
    OTMATH_OFFSET(Coverage, coverage);
    Short count;
    ValueRecord corrections[1];
  };

  struct TopAccentAttachment {
    OTMATH_OFFSET(Coverage, coverage);
    Short count;
    ValueRecord attachments[1];
  };

  struct KernInfoRecord {
    OTMATH_OFFSET(Kern, topRight);
    OTMATH_OFFSET(Kern, topLeft);
    OTMATH_OFFSET(Kern, bottomRight);
    OTMATH_OFFSET(Kern, bottomLeft);
  };

  struct KernInfo {
    OTMATH_OFFSET(Coverage, coverage);
    Short count;
    KernInfoRecord records[1];
  };

  struct Kern {
    UShort heightCount;
  private:
    ValueRecord records[1];
  public:

    inline ValueRecord& correctionHeight(int i) {
      return records[i];
    }

    inline ValueRecord& kernValue(int i) {
      return records[heightCount() + i];
    }
  };

  struct Variants {
    UShort minConnectorOverlap;
    OTMATH_OFFSET(Coverage, vertGlyphCoverage);
    OTMATH_OFFSET(Coverage, horizGlyphCoverage);
    UShort vertGlyphCount;
    UShort horizGlyphCount;

  private:
    Offset offsets[1];
  public:

    inline GlyphConstruction* vertGlyph(int i) {
      return (GlyphConstruction*)((char*)this + offsets[i]());
    }

    inline GlyphConstruction* horizGlyph(int i) {
      return (GlyphConstruction*)((char*)this + vertGlyphCount() + offsets[i]());
    }
  };

  struct GlyphVariant {
    GlyphID glyph;
    UShort advance;
  };

  struct GlyphConstruction {
    OTMATH_OFFSET(GlyphAssembly, glyphAssembly);
    UShort count;
    GlyphVariant variants[1];
  };

  struct GlyphPart {
    GlyphID glyph;
    UShort startConnectorLength;
    UShort endConnectorLength;
    UShort fullAdvance;
    UShort flags;
  };

  struct GlyphAssembly {
    ValueRecord italicsCorrection;
    UShort count;
    GlyphPart parts[1];
  };
}

