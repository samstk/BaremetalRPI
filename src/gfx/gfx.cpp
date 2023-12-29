#include <gfx/gfx.hpp>
#include <data/string.hpp>

#pragma region struct Point & Size
Point::Point() {
    this->x = 0;
    this->y = 0;
}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

Size::Size() {
    this->width = 0;
    this->height = 0;
}

Size::Size(int width, int height) {
    this->width = width;
    this->height = height;
}
#pragma endregion

#pragma region Rectangle
Rectangle::Rectangle() {
    this->x = 0;
    this->y = 0;
    this->width = 0;
    this->height = 0;
}

Rectangle::Rectangle(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}
#pragma endregion

#pragma region struct Color
Color::Color(char r, char g, char b) {
    this->data = 
        ((unsigned int)r) |
        ((unsigned int)g << 8) | 
        ((unsigned int)b << 16);
}

Color::Color(char r, char g, char b, char a) {
    this->data = ((unsigned int)a << 24) |
        ((unsigned int)r) |
        ((unsigned int)g << 8) | 
        ((unsigned int)b << 16);
}

char Color::getR() {
    return (char) ((this->data & 0x0000FF00) >> 8);
}
void Color::setR(char r) {
    this->data &= 0xFFFF00FF;
    this->data |= (unsigned int)r << 8;
}

char Color::getG() {
    return (char) ((this->data & 0x00FF0000) >> 16);
}
void Color::setG(char g) {
    this->data &= 0xFF00FFFF;
    this->data |= (unsigned int)g << 16;
}

char Color::getB() {
    return (char) ((this->data & 0xFF000000) >> 24);
}
void Color::setB(char r) {
    this->data &= 0x00FFFFFF;
    this->data |= (unsigned int)r << 24;
}

char Color::getA() {
    return (char) (this->data & 0xFF);
}
void Color::setA(char a) {
    this->data &= 0xFFFFFF00;
    this->data |= (unsigned int)a;
}

#pragma endregion

#pragma region struct Graphics
Graphics::Graphics(Framebuffer framebuffer) {
    this->buffer = framebuffer.buffer;
    this->bufferWidth = framebuffer.resolution.width;
    this->bufferHeight = framebuffer.resolution.height;
}

int Graphics::GetBufferSize() {
    return this->bufferWidth * this->bufferHeight * 4;
}

void Graphics::Fill(Color color) {
    unsigned int *buffer = this->buffer;
    int size = this->GetBufferSize();

    while(size-- > 0) {
        *buffer = color.data;
        buffer++;
    }
}

void Graphics::FillRectangle(Rectangle region, Color color) {
    unsigned int *buffer = this->buffer;

    int w = region.width;
    int h = region.height;

    // Calculate offset to starting print line
    buffer += (region.y * this->bufferWidth) + region.x;

    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            *buffer = color.data;
            buffer++;
        }

        // Calculate offset to next print line 
        buffer += (this->bufferWidth - w);
    }
}

void Graphics::DrawChar(PSF2Font* font, Point location, char c, Color color) {
    unsigned int *buffer = this->buffer + (location.y * this->bufferWidth + location.x);
    
    // PCF2 Fonts are defined 

    char* glyph = font->GetGlyph(c);
        
    char glyphRow = *glyph; 
    int skip = 8;

    for(unsigned int y = 0; y < font->header->characterHeight; y++) {
        for(unsigned int x = 0; x < font->header->characterWidth; x++) {
            if (skip == 0) {
                // Must swap to next glyph byte
                glyph++;
                glyphRow = *glyph; 
                skip = 8;
            }
            
            int mask = 1 << (7 - x % 8);
            if (glyphRow & mask) {
                // Must draw pixel
                *buffer = color.data;
            }
            

            skip--;
            buffer++;
        }

        // Must swap to next glyph byte on new row
        glyph++;
        glyphRow = *glyph; 
        skip = 8;
        buffer += this->bufferWidth - font->header->characterWidth;
    }
};

void Graphics::DrawString(PSF2Font* font, Point startLocation, String text, Color color) {
    int x = startLocation.x;
    int y = startLocation.y;
    int length = text.GetLength();
    char* data = text.data;
    char c;
    while (length > 0) {
        c = *data;

        if (c == '\n') {
            x = startLocation.x;
            y += font->header->characterHeight;
        }

        if (font->HasCharacterGlyph(c)) {
            this->DrawChar(font, Point(x,y), c, color);
        } else {
            // Assumes ? is within the font
            this->DrawChar(font, Point(x,y), '?', color);
        }

        x += font->header->characterWidth;
        data++;
        length--;
    }
}
#pragma endregion