#ifndef MONITIO_LOG_H
#define MONITIO_LOG_H

/**
 * Monitio/shared - mlog.h
 *   - A simple C Library for quick logging functions
 *     that actually make sense.
 *
 * Required:
 *   - A C standard library (recommended: glibc or musl)
 *   - A C compiler that supports the C standard library of your choice
 *   - A TrueColor (24-bit) ANSI escape supported terminal ( Kitty, Alacritty
 *                                                           GNOME Terminal,
 *                                                           xfce4-terminal,
 *                                                           WezTerm, Konsole,
 *                                                           Ghostty, iTerm2,
 *                                                           Warp, Windows Terminal
 *                                                           and really any
 *                                                           other modern
 *                                                           terminal you
 *                                                           can think of. )
 *
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

// -- Types and function prototypes

typedef struct {
  unsigned char r, g, b, a;
} ML_RGBA;

typedef struct {
  float h; // 0-360
  float s; // 0-1
  float l; // 0-1
} ML_HSL;

typedef struct {
  char str[8];
} ML_HEX;

static inline float _ml_max(float a, float b);
static inline float _ml_min(float a, float b);
static inline float _ml_abs(float v);
static inline float _ml_fmod(float a, float b);
static inline void _ml_apply_reset();
static inline void _ml_apply_color(ML_RGBA c);
static inline void _ml_print_prefix();
static inline void _ml_print_sym(const char* sym, const char* hex_color);
static inline ML_HEX ml_hex(const char* hex_str);
static inline ML_RGBA ml_hex_to_rgba(ML_HEX hex);
static inline ML_HSL ml_hex_to_hsl(ML_HEX hex);
static inline ML_HEX ml_hsl_to_hex(ML_HSL hsl);
static inline ML_RGBA ml_hsl_to_rgba(ML_HSL hsl);
static inline ML_HSL ml_rgba_to_hsl(ML_RGBA rgba);
static inline ML_HEX ml_rgba_to_hex(ML_RGBA rgba);
static inline void ml_err(bool quit, const char* msg, ...);
static inline void ml_wrn(const char* msg, ...);
static inline void ml_inf(const char* msg, ...);
static inline void ml_suc(const char* msg, ...);

// -- Internal helpers

static inline float _ml_max(float a, float b) { return (a > b) ? a : b; }
static inline float _ml_min(float a, float b) { return (a < b) ? a : b; }
static inline float _ml_abs(float v) { return v < 0 ? -v : v; }
static inline float _ml_fmod(float a, float b) {
  if (b == 0) { return 0; }
  return (a - (int)(a / b) * b);
}

// --- Display helpers

static inline void _ml_apply_reset() { printf("\x1b[0m"); }
static inline void _ml_apply_color(ML_RGBA c) { printf("\x1b[38;2;%d;%d;%dm", c.r, c.g, c.b); }
static inline void _ml_print_prefix() {
  time_t rawtime;
  struct tm *info;
  char t_buf[12], d_buf[12];
  time(&rawtime);
  info = localtime(&rawtime);
  strftime(t_buf, sizeof(t_buf), "%H:%M:%S", info);
  strftime(d_buf, sizeof(d_buf), "%d/%m/%Y", info);

  #if defined(ML_SHOWTIME) && defined(ML_SHOWDATE)
  #if defined(ML_TIME_FIRST)
  printf("[%s] [%s] ", t_buf, d_buf);
  #else
  printf("[%s] [%s] ", d_buf, t_buf);
  #endif
  #elif defined(ML_SHOWTIME)
  printf("[%s] ", t_buf);
  #elif defined(ML_SHOWDATE)
  printf("[%s] ", d_buf);
  #endif
}
static inline void _ml_print_sym(const char* sym, const char* hex_color) {
  printf("[");
  _ml_apply_color(ml_hex_to_rgba(ml_hex(hex_color)));
  printf("%s", sym);
  _ml_apply_reset();
  printf("] ");
}

// -- Actually usable API functions

// --- Color conversion functions

// HEX bridge (string -> ML_HEX type)
static inline ML_HEX ml_hex(const char* hex_str) {
  ML_HEX h;
  snprintf(h.str, sizeof(h.str), "%s", hex_str);
  return h;
}

// HEX -> RGBA
static inline ML_RGBA ml_hex_to_rgba(ML_HEX hex) {
  ML_RGBA color = {0, 0, 0, 255};
  const char* p = hex.str;
  if (p[0] == '#') { p++; }
  sscanf(p, "%02hhx%02hhx%02hhx", &color.r, &color.g, &color.b);
  return color;
}

// HEX -> HSL
static inline ML_HSL ml_hex_to_hsl(ML_HEX hex) {
  return ml_rgba_to_hsl(ml_hex_to_rgba(hex));
}

// HSL -> HEX
static inline ML_HEX ml_hsl_to_hex(ML_HSL hsl) {
  return ml_rgba_to_hex(ml_hsl_to_rgba(hsl));
}

// HSL -> RGBA
static inline ML_RGBA ml_hsl_to_rgba(ML_HSL hsl) {
  float c = (1.0f - _ml_abs(2.0f * hsl.l - 1.0f)) * hsl.s;
  float h_p = hsl.h / 60.0f;
  float x = c * (1.0f - _ml_abs(_ml_fmod(h_p, 2.0f) - 1.0f));
  float m = hsl.l - c / 2.0f;
  float r = 0, g = 0, b = 0;

  if (h_p < 1) {
    r = c; g = x; b = 0;
  } else if (h_p < 2) {
    r = x; g = c; b = 0;
  } else if (h_p < 3) {
    r = 0; g = c; b = x;
  } else if (h_p < 4) {
    r = 0; g = x; b = c;
  } else if (h_p < 5) {
    r = x; g = 0; b = c;
  } else if (h_p < 6) {
    r = c; g = 0; b = x;
  }

  return (ML_RGBA){
    (unsigned char)((r + m) * 255),
    (unsigned char)((g + m) * 255),
    (unsigned char)((b + m) * 255),
    255
  };
}

// RGBA -> HEX
static inline ML_HEX ml_rgba_to_hex(ML_RGBA rgba) {
  ML_HEX h;
  snprintf(h.str, sizeof(h.str), "#%02X%02X%02X", rgba.r, rgba.g, rgba.b);
  return h;
}

// RGBA -> HSL
static inline ML_HSL ml_rgba_to_hsl(ML_RGBA rgba) {
  ML_HSL hsl;
  float r = rgba.r / 255.0f;
  float g = rgba.g / 255.0f;
  float b = rgba.b / 255.0f;

  float max = _ml_max(r, _ml_max(g, b));
  float min = _ml_min(r, _ml_min(g, b));
  float delta = max - min;

  hsl.l = (max + min) / 2.0f;

  if (delta == 0) {
    hsl.h = 0;
    hsl.s = 0;
  } else {
    hsl.s = (hsl.l <= 0.5f) ? (delta / (max + min)) : (delta / (2.0f - max - min));
    if (max == r) {
      hsl.h = 60.0f * _ml_fmod(((g - b) / delta), 6.0f);
    } else if (max == g) {
      hsl.h = 60.0f * (((b - r) / delta) + 2.0f);
    } else {
      hsl.h = 60.0f * (((r - g) / delta) + 4.0f);
    }
    if (hsl.h < 0) { hsl.h += 360.0f; }
  }
  return hsl;
}

// --- Display logging functions

static inline void ml_err(bool quit, const char* msg, ...) {
  _ml_print_prefix();
  _ml_print_sym("!", "#E31D39");
  printf("Error: ");
  va_list args;
  va_start(args, msg);
  vprintf(msg, args);
  va_end(args);
  _ml_apply_reset();
  printf("\n");
  if (quit) { exit(1); }
}

static inline void ml_wrn(const char* msg, ...) {
  _ml_print_prefix();
  _ml_print_sym("?", "#F5CB3A");
  printf("Warning: ");
  va_list args;
  va_start(args, msg);
  vprintf(msg, args);
  va_end(args);
  _ml_apply_reset();
  printf("\n");
}

static inline void ml_inf(const char* msg, ...) {
  _ml_print_prefix();
  _ml_print_sym("*", "#3AE6F5");
  printf("Note: ");
  va_list args;
  va_start(args, msg);
  vprintf(msg, args);
  va_end(args);
  _ml_apply_reset();
  printf("\n");
}

static inline void ml_suc(const char* msg, ...) {
  _ml_print_prefix();
  _ml_print_sym("+", "#3AF547");
  printf("Success: ");
  va_list args;
  va_start(args, msg);
  vprintf(msg, args);
  va_end(args);
  _ml_apply_reset();
  printf("\n");
}

#endif // MONITIO_LOG_H
