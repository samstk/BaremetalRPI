#include <hardware/mailbox.hpp>
#include <system.hpp>
#include <commons.hpp>
#pragma region struct Mailbox

Mailbox::Mailbox() {
    this->ioAddress = (uint *)(PERIPHERAL_BASE + MAIL_BASE + MAIL_TAGS + MAIL_WRITE);
    this->data = (uint *) mbox;
    this->Reset();
}

void Mailbox::Reset() {
    this->dataOffset = 0;
    this->dataSize = 0;
    this->AddData(0); // Buffer Size
    this->AddData(0); // Request/Response Code
}

uint* Mailbox::AddData(uint data) {
    uint *addr = this->data + this->dataOffset;
    this->data[this->dataOffset] = data;
    this->dataSize += 4;
    this->dataOffset++;
    return addr;
}

uint* Mailbox::AddRequestProperty(uint code, uint arg1) {
    this->AddData(code);
    this->AddData(4); // Value Buffer Size
    this->AddData(4); // 1-bit (REQ/RESP indicator), 31-bit Value Length
    
    uint *addr = this->AddData(arg1);
    return addr;
}

uint* Mailbox::AddRequestProperty(uint code, uint arg1, uint arg2) {
    this->AddData(code);
    this->AddData(8); // Value Buffer Size
    this->AddData(8); // 1-bit (REQ/RESP indicator), 31-bit Value Length
    uint *addr = this->AddData(arg1);
    this->AddData(arg2);
    return addr;
}

uint* Mailbox::AddRequestProperty(uint code, uint arg1, uint arg2,
    uint arg3) {
    this->AddData(code);
    this->AddData(12); // Value Buffer Size
    this->AddData(12); // 1-bit (REQ/RESP indicator), 31-bit Value Length
    uint *addr = this->AddData(arg1);
    this->AddData(arg2);
    this->AddData(arg3);
    return addr;
}

uint* Mailbox::AddRequestProperty(uint code, uint arg1, uint arg2,
    uint arg3, uint arg4) {
    this->AddData(code);
    this->AddData(16); // Value Buffer Size
    this->AddData(16); // 1-bit (REQ/RESP indicator), 31-bit Value Length
    uint *addr = this->AddData(arg1);
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
    *(this->ioAddress) = (uint)((ulong)this->data + sizeof(int) * 2);
}

#pragma endregion