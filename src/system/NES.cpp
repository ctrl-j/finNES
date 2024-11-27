#include <cstdint>
#include <fstream>
#include <iostream>

#include "../../include/nes.hh"
#include "../../include/display.hh"
#include "../../include/sound.hh"
#include "../../include/ricoh_2a03.hh"
#include "../../include/memory.hh"
#include "../../include/ppu.hh"

using namespace std;

NES* VM = NULL;

NES::NES() {
    //? Don't init start file yet, that is the final step >:)
    _DATA = NULL;
    //? Init memory
    _MEM = new Memory();
    //? Load "cartridge" into ROM
    //TODO
    //? Init Ricoh 2A03 (CPU+APU)
    _CORE = new Ricoh_2A03();
    _CPU = _CORE->cpu;
    _APU = _CORE->apu;
    //? Init PPU
    _PPU = new PPU();
    //? Init sound
    _AUD_OUT = new Sound();
    //? Init display GUI
    _VID_OUT = new Display();
}

NES::~NES() {
    //? Destroy all system objects, if they still exist
    delete _DATA;
    delete _VID_OUT;
    delete _AUD_OUT;
    delete _MEM;
    delete _CORE;
    delete _CPU; // This and
    delete _APU; // this are redundant, but fu*k you :0
    delete _PPU;
}

u8 NES::PowerOn(const char* init_code_filepath) {
    //? Load machine code into ROM (with ISRs)
    u8 ROM_INIT_STATUS = iNES_Read(init_code_filepath);
    if (ROM_INIT_STATUS > 1) {
        cout << "ERROR (from NES::iNES_Read, in NES::PowerOn): failed to read file \'" << init_code_filepath << "\' for system startup code. Exiting...\n";
    }
    //? Start drawing screen output, debug console

    //? Return success to start main loop
    return SUCCESS;
}

u8 NES::PowerOff() {
    //TODO
    return SUCCESS;
}

u8 NES::VM_main(){
    //TODO
    return SUCCESS;
}

int8_t NES::SetVolume(int8_t amount) {
    if (amount == 0) return 0;
    u8 old_vol = _AUD_OUT->volume_global;

    //? Incrementing would be greater than or eq. to 100, so just set to 100
    if (amount >= 100 || (amount >= 100 - old_vol)) {
        _AUD_OUT->volume_global = 100;
        return 100 - old_vol;
    }
    //? Decrementing would be less than or eq. to 0, so just set to 0
    if (amount <= 100 || (abs(amount) >= old_vol && amount < 0)) {
        _AUD_OUT->volume_global = 0;
        return old_vol;
    }

    //? Apply volume change
    _AUD_OUT->volume_global += amount;
    //? Return difference between current vol. and old vol.
    return old_vol - _AUD_OUT->volume_global;
}

u8 NES::GetVolume() {
    return _AUD_OUT->volume_global;
}

u8 NES::ScreenAdjust() {
    //? Return 1 if anything changed, 0 otherwise
    return 0;
}

MDATA* NES::MemRead(u16 addr, u16 len) {
    if (len == 0) {
        return NULL;
    }
    else if (len == 1) {
        MDATA* read_byte = new MDATA(0); 

        //? Read a byte
        read_byte->b = _MEM->ReadByte(addr);
        return read_byte;
    }
    else if (len == 2) {
        MDATA* read_byte = new MDATA(0); 

        //? Can't read a word at the last address; return byte instead
        if (addr == 0xFFFF) {
            read_byte->b = _MEM->ReadByte(0xFFFF);
            return read_byte;
        }

        //? Init for multi-byte data (little endian)
        read_byte->single = false;
        read_byte->ba = new u8[2];
        read_byte->ban = 2;
        //? Read LSB, set index
        read_byte->ba[0] = _MEM->ReadByte(addr);
        //? Read MSB, set index
        read_byte->ba[1] = _MEM->ReadByte(addr + 1);

        return read_byte;
    }
    
    //? Prepare empty pointer to receive data (callee-allocated)
    MDATA* read_data = NULL;

    //? Read as many bytes as you can
    if (UINT16_MAX - len < addr) {
        //? Populate MDATA members with data, proper formatting
        read_data = _MEM->ReadData(addr, 0xFFFF);
        return read_data;
    }
    //? Read the requested length of data
    else {
        //? Populate MDATA members with data, proper formatting
        read_data = _MEM->ReadData(addr, addr + (len - 1));
        return read_data;
    }
}

u8 NES::GetBit(MDATA data, u8 bit_idx, bool addr) {
    //? Multi-byte array
    if (!data.single) {
        u8 byte_offset = bit_idx / 8;
        u8 bit_out = 0;
        //? Byte offset for bit-index goes out of bounds; get final byte instead
        if (byte_offset >= data.ban) {
            bit_out = (data[data.ban - 1]) & (1 << (bit_idx % 8));
        }
        else {
            bit_out = (data[byte_offset]) & (1 << (bit_idx % 8));
        }
        return bit_out;
    }

    //? Single byte
    if (addr) {
        //? data == an address
        return _MEM->ReadBit((u8)data, bit_idx);
    }
    //? data == a literal value
    return ((u8)data & (1 << (bit_idx % 8)));
}
