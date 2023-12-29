#ifndef _H_STRING
#define _H_STRING

/// @brief Values that determine how a particular value is read as a string
enum StringConversionFormat {
    /// @brief The value is converted in 1's and 0's (e.g. 10001100)
    BINARY,
    /// @brief The value is converted to the original representation (e.g. 12)
    ORIGINAL,
    /// @brief The value is converted to the hex representation (e.g. 0xFF102938)
    HEX
};

/// @brief A wrapper class for character buffers
struct String {
    public:
        /// @brief The actual data of the string
        ///        (not to be written unless with newly allocated string)
        char* data;
        
    private:
        /// @brief The length of the string 
        ///        (left -1 if not read and \0 exists)
        int _length = -1;

        /// @brief True if an allocation was necessary for the string.
        bool _allocated = false;

    public:
        /// @brief Constructs a string from the given original
        /// @param string The original string
        String(const String &string);

        /// @brief Constructs a string with a newly allocated character buffer
        /// @param length The character buffer's size and length of the string
        String(int length);

        /// @brief Constructs a string from a character buffer
        /// @param data The character buffer (\0 must exist at the end of the string or
        ///             the length of the string must be preset)
        String(char* data);

        /// @brief Constructs a string from a character buffer
        /// @param data The character buffer (\0 must exist at the end of the string or
        ///             the length of the string must be preset)
        String(const char* data);

        /// @brief Constructs a string from a character buffer
        /// @param data The character buffer
        /// @param length The length of the string
        String(char* data, int length);

        /// @brief Constructs a string from a character buffer
        /// @param data The character buffer
        /// @param length The length of the string
        String(const char* data, int length);

        ~ String();

        String operator +(String other);
        
        /// @brief Gets the length of the string
        /// @return The length of the string
        int GetLength();

        /// @brief Removes a section of the text in a new string
        /// @param fromIndex The index to remove from
        /// @return The new string with the removed section
        String Remove(int fromIndex);

        /// @brief Takes a substring of the text in a new string
        /// @param fromIndex The index to take from
        /// @return The new string with the sub section
        String Substring(int fromIndex);

        /// @brief Takes a substring of the text in a new string
        /// @param fromIndex The index to take from
        /// @param length The number of bytes to take
        /// @return The new string with the sub section
        String Substring(int fromIndex, int length);

        /// @brief Converts an integer to a string value.
        /// @param value The value to convert
        /// @param format The format to convert to
        /// @return The converted string
        static String ParseInt(int value, StringConversionFormat format);
        
        /// @brief Converts an integer to a string value.
        /// @param value The value to convert
        /// @param format The format to convert to
        /// @param bits The number of bits storing the value
        /// @return The converted string
        static String ParseLong(long value, StringConversionFormat format, int bits=64);

        /// @brief Converts an integer to a string value.
        /// @param value The value to convert
        /// @param format The format to convert to
        /// @return The converted string
        static String ParseInt(unsigned int value, StringConversionFormat format);
        
        /// @brief Converts an integer to a string value.
        /// @param value The value to convert
        /// @param format The format to convert to
        /// @param bits The number of bits storing the value
        /// @return The converted string
        static String ParseLong(unsigned long value, StringConversionFormat format, int bits=64);
};

#endif