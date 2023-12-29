#include <data/string.hpp>
#include <system.hpp>

#pragma region struct String

/// @brief Measures the length of a system-defined string
/// @param data The system-defined string
/// @return The length of the string
int measureString(char* data) {
    // Calculate length
    int length = 0;
    while (*data != '\0') {
        data++;
        length++;
    }
    return length;
}

String::~String() {
    if (this->_allocated) {
        free(this->data);
    }
}

String::String(int length) {
    this->data = (char*) malloc(length);
    this->_length = length;
    this->_allocated = true;
}

String::String(const String &string) {
    int length = string._length == -1 ? measureString(string.data) : string._length;
    this->data = (char*) malloc(length);
    this->_length = length;
    this->_allocated = true;
    memcpy(this->data, string.data, length);
}

String::String(char *data) {
    // Store direct reference as it should not change.
    this->data = data;
}

String::String(const char *data) {
    // Store direct reference as it should not change.
    this->data = (char*)data;
}

String::String(char *data, int length) : String::String(data) {
    this->_length = length;
}

String::String(const char *data, int length) : String::String(data) {
    this->_length = length;
}

String String::operator+(String other) {
    int thisLength = this->GetLength();
    int otherLength = other.GetLength();
    
    String combined = String(thisLength + otherLength);
    char* dest = combined.data;
    memcpy(dest, this->data, thisLength);
    dest += thisLength;
    memcpy(dest, other.data, otherLength);
    return combined;
}

int String::GetLength() {
    if (this->_length == -1) { 
        this->_length = measureString(this->data);
    }
    return this->_length;
}

String String::Remove(int fromIndex) {
    return String(this->data, fromIndex);
}

String String::Substring(int fromIndex) {
    return String(this->data + fromIndex, this->GetLength() - fromIndex);
}

String String::Substring(int fromIndex, int length) {
    return String(this->data + fromIndex, length);
}


String ParseIntBinary(unsigned int value, int bits) {
    String string = String(bits);
    for(int i = 0; i < bits; i ++) {
        if (value & (1 << (bits - 1 - i))) {
            string.data[i] = '1';
        } else {
            string.data[i] = '0';
        }
    }
    return string;
}

String ParseIntHex(unsigned int value, int bits) {
    int digits = bits / 4;
    String string = String(digits);
    char* data = string.data;
    for(int i = 0; i < digits; i++) {
        int shift = (bits - 4 - i * 4);
        int mask = 0b1111 << shift;
        unsigned int digit = (value & mask) >> shift;
        if (digit < 10) {
            *data = digit + '0';
        } else {
            *data = (digit - 10) + 'A';
        }
        data++;
    }
    return string;
}

String String::ParseInt(int value, StringConversionFormat format) {
    if (format == StringConversionFormat::BINARY) {
        return ParseIntBinary((unsigned int)value, 32);
    } else if (format == StringConversionFormat::HEX) {
        return ParseIntHex((unsigned int)value, 32);
    }
    return String::ParseLong((long)value, format, 32);
}

String String::ParseInt(unsigned int value, StringConversionFormat format) {
    return String::ParseLong((long)value, format, 32);
}

String ParseLongBinary(unsigned long value, int bits) {
    String string = String(bits);
    for(int i = 0; i < bits; i ++) {
        if (value & (1 << (bits - 1 - i))) {
            string.data[i] = '1';
        } else {
            string.data[i] = '0';
        }
    }
    return string;
}

String ParseLongHex(unsigned long value, int bits) {
    int digits = bits / 4;
    String string = String(digits);
    char* data = string.data;
    for(int i = 0; i < digits; i++) {
        int shift = (bits - 4 - i * 4);
        int mask = 0b1111 << shift;
        unsigned int digit = (value & mask) >> shift;
        if (digit < 10) {
            *data = digit + '0';
        } else {
            *data = (digit - 10) + 'A';
        }
        data++;
    }
    return string;
}

String String::ParseLong(long value, StringConversionFormat format, int bits) {
    // Use correct conversion method
    if (format == StringConversionFormat::BINARY) {
        return ParseLongBinary((unsigned long)value, bits);
    } else if (format == StringConversionFormat::HEX) {
        return ParseLongHex((unsigned long)value, bits);
    }

    int length = 0;
    bool isNegative = value < 0;

    // Calculate length of representation
    if (isNegative) {
        length++;
        value = -value;
    }

    int displayValue = value;
    do {
        length++;
        value /= 10;
    } while (value > 0);

    // Create string from integer value
    String string = String(length);
    char* data = string.data + length - 1;
    value = displayValue;
    do {
        *data = (char)(value % 10) + '0';
        data--;
        value /= 10;
    } while (value > 0);

    if (isNegative)
        *data = '-';
    return string;
}

String String::ParseLong(unsigned long value, StringConversionFormat format, int bits) {
    // Use correct conversion method
    if (format == StringConversionFormat::BINARY) {
        return ParseLongBinary(value, bits);
    } else if (format == StringConversionFormat::HEX) {
        return ParseLongHex(value, bits);
    }

    int length = 0;

    // Calculate length of representation
    unsigned long displayValue = value;
    do {
        length++;
        value /= 10;
    } while (value > 0);

    // Create string from integer value
    String string = String(length);
    char* data = string.data + (length - 1);
    value = displayValue;
    do {
        *data = (char)(value % 10) + '0';
        data--;
        value /= 10;
    } while (value > 0);
    return string;
}
#pragma endregion