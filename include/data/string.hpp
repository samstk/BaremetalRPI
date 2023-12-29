#ifndef _H_STRING
#define _H_STRING

/// @brief A wrapper class for character buffers
struct String {
    public:
        /// @brief The actual data of the string
        ///        (not to be written)
        char* data;
        
    private:
        /// @brief The length of the string 
        ///        (left -1 if not read and \0 exists)
        int _length = -1;

    public:

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
};

#endif