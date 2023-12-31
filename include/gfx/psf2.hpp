#ifndef _H_PSF2
#define _H_PSF2
#include <commons.hpp>
struct PSF2Header {
    public:
        uint magic;
        uint version;
        uint headerSize;
        uint flags;
        uint length;
        uint glyphSize;
        uint characterHeight;
        uint characterWidth;        
};
#endif