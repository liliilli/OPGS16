#ifndef OPGS16_HELPER_TYPE_COLOR32_H
#define OPGS16_HELPER_TYPE_COLOR32_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/color32.h
///
/// @brief 8-bit color type which stores 4 components (r, g, b, a)
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-04 Create file.
///

#include <cstdint>
/// ::opgs16::DClamp<uint8_t>
#include <Helper/Type/clamp.h>
#include <Helper/Type/color.h>

namespace opgs16 {

///
/// @class DColor32
/// @brief 8-bit color type which stores 4 components (r, g, b, a)
/// (h, s, v) does not support but can be converted calling HsvToRgb().
///
struct DColor32 final {
private:
  using TChannel = DClamp<uint8_t>;

  TChannel r = {};
  TChannel g = {};
  TChannel b = {};
  TChannel a = {};

public:
  constexpr DColor32() = default;
  ~DColor32() = default;

  explicit DColor32(uint8_t r, uint8_t g, uint8_t b) noexcept :
      DColor32{r, g, b, 0xFF} {}

  explicit DColor32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }

  DColor32(const DColor32&) = default;
  DColor32& operator=(const DColor32&) = default;
  DColor32(DColor32&&) = default;
  DColor32& operator=(DColor32&&) = default;

  //!
  //! Methods
  //!

  constexpr bool IsOpaque() const noexcept {
    return this->a == static_cast<uint8_t>(0xFF);
  }

  constexpr void SetOpqaue() noexcept {
    this->a = 0xFF;
  }

  constexpr float GetGrayScale() const noexcept {
    return 0.2126f * this->r() + 0.7152f * this->g() + 0.0722f * this->b();
  }

  constexpr uint8_t R() const noexcept {
    return this->r();
  }

  constexpr uint8_t G() const noexcept {
    return this->g();
  }

  constexpr uint8_t B() const noexcept {
    return this->b();
  }

  constexpr uint8_t A() const noexcept {
    return this->a();
  }

  //!
  //! Operator overloading
  //!

  //!
  //! Conversion operators
  //!

  ///
  /// @brief Can be convert DColor explicitly.
  ///
  constexpr operator DColor() const noexcept {
    return DColor{
        static_cast<float>(r()) / 0xFF,
        static_cast<float>(g()) / 0xFF,
        static_cast<float>(b()) / 0xFF,
        static_cast<float>(a()) / 0xFF};
  }

  //!
  //! Statics
  //!

  static const DColor32 Aqua;
  static const DColor32 Black;
  static const DColor32 Blue;
  static const DColor32 DarkRed;
  static const DColor32 DarkGray;
  static const DColor32 DarkGreen;
  static const DColor32 Gold;
  static const DColor32 Gray;
  static const DColor32 Green;
  static const DColor32 Magenta;
  static const DColor32 Orange;
  static const DColor32 Purple;
  static const DColor32 Red;
  static const DColor32 White;
  static const DColor32 Yellow;

};

} /// ::opgs16 namespace

#endif /// OPGS16_HELPER_TYPE_COLOR32_H