#ifndef _H_GFX
#define _H_GFX
struct Framebuffer;

struct Color {
    public:
        /// @brief The 32-bit integer representing this color
        unsigned int data;

        /// @brief Initializes a new color with the given components
        /// @param r The red component of the color
        /// @param g The green component of the color
        /// @param b The blue component of the color
        Color(char r, char g, char b);
        
        /// @brief Initializes a new color with the given components
        /// @param r The red component of the color
        /// @param g The green component of the color
        /// @param b The blue component of the color
        /// @param a The alpha transparency component of the color
        Color(char r, char g, char b, char a);

        /// @brief Gets the red component of this color
        /// @return The new red value
        char getR();
        
        /// @brief Sets the red component of this color
        /// @param r The red value
        void setR(char r);

        /// @brief Gets the green component of this color
        /// @return The green value
        char getG();

        /// @brief Sets the green component of this color
        /// @param g The new green value
        void setG(char g);

        /// @brief Gets the blue component of this color
        /// @return The blue value
        char getB();

        /// @brief Sets the blue component of this color
        /// @param b The new blue value
        void setB(char b);

        /// @brief Gets the alpha transparency component of this color
        /// @return The alpha value
        char getA();
        
        /// @brief Sets the alpha transparency component of this color
        /// @param a The new transparency value 
        void setA(char a);
};

/// @brief A size of a region
struct Size {
    public:
        /// @brief The width of the region
        int width;
        
        /// @brief The height of the region
        int height;

        /// @brief Creates a zeroed size
        Size();

        /// @brief Creates a size with the given dimensions
        /// @param width The width of the region
        /// @param height The height of the region
        Size(int width, int height);
};

/// @brief A point representing a 2D co-ordinate
struct Point {
    public:
        /// @brief The X-coordinate of the point
        int x;
        /// @brief The Y-coordinate of the point
        int y;

        /// @brief Creates a point with zeroed coordinates
        Point();

        /// @brief Creates a point with the given coordinates
        /// @param x The x-coordinate of the point
        /// @param y The y-coordinate of the point
        Point(int x, int y);
};

/// @brief A rectangular region
struct Rectangle {
    public:
        /// @brief The x coordinate of the rectangle
        int x;
        /// @brief The y coordinate of the rectangle
        int y;
        /// @brief The width of the rectangle
        int width;
        /// @brief The height of the rectangle
        int height;

        /// @brief Initializes an instance of a zeroed rectangular region
        Rectangle();

        /// @brief Initializes an instance of a rectangular region
        /// @param x The x coordinate of the rectangle
        /// @param y The y coordinate of the rectangle
        /// @param width The width of the rectangle
        /// @param height The height of the rectangle
        Rectangle(int x, int y, int width, int height);
};

struct Graphics {
    public: 
        /// @brief The address of the buffer
        unsigned int *buffer;
        
        /// @brief The width of the region the buffer contains
        int bufferWidth;

        /// @brief The height of the region the buffer contains
        int bufferHeight;

        /// @brief Creates a graphics handle for the framebuffer 
        /// @param framebuffer The framebuffer to draw on
        Graphics(Framebuffer framebuffer);

        /// @brief Calculates the total size of the buffer
        /// @return The size of the buffer
        int getBufferSize();

        /// @brief Fills the buffer with the given colour
        /// @param color The color to fill with
        void fill(Color color);

        /// @brief Fills a region of the buffer with the given colour
        /// @param region The region to fill
        /// @param color The color to fill with
        void fillRectangle(Rectangle region, Color color);
};

#include <hardware/framebuffer.hpp>
#endif