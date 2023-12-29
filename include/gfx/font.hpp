#ifndef _H_FONT
#define _H_FONT
#include <gfx/psf2.hpp>

/// @brief A font to draw text with (ASCII support only)
struct PSF2Font {
    public:
        /// @brief The location to the font's header and data
        PSF2Header* header;
        
        /// @brief The location Mappings for ASCII characters
        char* mappings[128];

        /// @brief Creates an uninitialized font
        PSF2Font ();

        /// @brief Initialises the font with the given PSF2 buffer
        /// @param buffer  the buffer in PSF2 format.
        PSF2Font (char *buffer);

        /// @brief Gets whether a glyph exists for the character
        /// @param c The character to check
        /// @return true if a glyph exists
        bool HasCharacterGlyph(char c);

        /// @brief Gets the glyph data buffer for the character
        /// @param c The character to get the glyph for
        /// @return The buffer for the character's glyph
        char* GetGlyph(char c);
};

#endif