#ifndef _H_FRAMEBUFFER
#define _H_FRAMEBUFFER
#include <gfx/gfx.hpp>
#include <commons.hpp>
struct Framebuffer {
    public:
        /// @brief The physical address to the framebuffer
        uint* buffer;

        /// @brief The resolution of the framebuffer as initialized.
        Size resolution;

        /// @brief Creates an uninitialized framebuffer reference.
        Framebuffer();

        /// @brief Initialises the RPI3A+ framebuffer with the given resolution
        /// @param width The width of the framebuffer
        /// @param height The height of the framebuffer
        Framebuffer(int width, int height);
};

#endif