#include <resources/systemfont.hpp>

void setSystemFont(PSF2Font systemFont) {
  _systemFont = systemFont;
}

PSF2Font getSystemFont() {
  return _systemFont;
}