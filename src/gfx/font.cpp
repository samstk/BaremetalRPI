#include <gfx/font.hpp>
#include <gfx/psf2.hpp>
#include <system.hpp>

#pragma region Font
PSF2Font::PSF2Font(char* buffer) {
    PSF2Header* header = (PSF2Header*) buffer;
    header->magic = magic(header->magic);
    this->header = header;
    
    // Populate ASCII buffer mappings
    for(int i = 0; i < 128 && i < (int)header->length; i++) {
        this->mappings[i] = NULL;
        char* buffer = (char*)header + header->headerSize;
        this->mappings[i] = buffer + i * header->glyphSize;
    }
}

PSF2Font::PSF2Font() {

}

bool PSF2Font::HasCharacterGlyph(char c) {
    return c < this->header->length && this->mappings[(int)c] != NULL;
}

char* PSF2Font::GetGlyph(char c) {
    if (c >= this->header->length)
    {
        return NULL;
    }

    return mappings[(int)c];
}
#pragma endregion
