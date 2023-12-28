#include <hardware/mailbox.hpp>
#include <board.hpp>

#pragma region struct Mailbox

Mailbox::Mailbox() {
    this->ioAddress = (unsigned int *)(PERIPHERAL_BASE + MAIL_BASE + MAIL_TAGS + MAIL_WRITE);
    this->data = (unsigned int *) mbox;
    this->Reset();
}

void Mailbox::Reset() {
    this->dataOffset = 0;
    this->dataSize = 0;
    this->AddData(0); // Buffer Size
    this->AddData(0); // Request/Response Code
}

unsigned int* Mailbox::AddData(unsigned int data) {
    unsigned int *addr = this->data + this->dataOffset;
    this->data[this->dataOffset] = data;
    this->dataSize += 4;
    this->dataOffset++;
    return addr;
}

unsigned int* Mailbox::AddRequestProperty(unsigned int code, unsigned int arg1) {
    this->AddData(code);
    this->AddData(4); // Value Buffer Size
    this->AddData(4); // 1-bit (REQ/RESP indicator), 31-bit Value Length
    
    unsigned int *addr = this->AddData(arg1);
    return addr;
}

unsigned int* Mailbox::AddRequestProperty(unsigned int code, unsigned int arg1, unsigned int arg2) {
    this->AddData(code);
    this->AddData(8); // Value Buffer Size
    this->AddData(8); // 1-bit (REQ/RESP indicator), 31-bit Value Length
    unsigned int *addr = this->AddData(arg1);
    this->AddData(arg2);
    return addr;
}

unsigned int* Mailbox::AddRequestProperty(unsigned int code, unsigned int arg1, unsigned int arg2,
    unsigned int arg3) {
    this->AddData(code);
    this->AddData(12); // Value Buffer Size
    this->AddData(12); // 1-bit (REQ/RESP indicator), 31-bit Value Length
    unsigned int *addr = this->AddData(arg1);
    this->AddData(arg2);
    this->AddData(arg3);
    return addr;
}

unsigned int* Mailbox::AddRequestProperty(unsigned int code, unsigned int arg1, unsigned int arg2,
    unsigned int arg3, unsigned int arg4) {
    this->AddData(code);
    this->AddData(16); // Value Buffer Size
    this->AddData(16); // 1-bit (REQ/RESP indicator), 31-bit Value Length
    unsigned int *addr = this->AddData(arg1);
    this->AddData(arg2);
    this->AddData(arg3);
    this->AddData(arg4);
    return addr;
}

void Mailbox::Pack() {
    this->AddData(0); // End tag
    this->data[0] = this->dataSize;
}

void Mailbox::Write() {
    *(this->ioAddress) = (unsigned int)((unsigned long)this->data + sizeof(int) * 2);
}

#pragma endregion