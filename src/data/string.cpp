#include <data/string.hpp>

#pragma region struct String
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

int String::GetLength() {
    if (this->_length == -1) { 
        // Calculate length
        int length = 0;
        char* dat = this->data;
        while (*dat != '\0') {
            dat++;
            length++;
        }
        this->_length = length;
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
#pragma endregion