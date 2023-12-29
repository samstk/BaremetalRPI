#ifndef _H_PSF2
#define _H_PSF2
struct PSF2Header {
    public:
        unsigned int magic;
        unsigned int version;
        unsigned int headerSize;
        unsigned int flags;
        unsigned int length;
        unsigned int glyphSize;
        unsigned int characterHeight;
        unsigned int characterWidth;        
};
#endif