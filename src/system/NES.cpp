#include <cstdint>
#include <fstream>
#include <iostream>

#include "../../include/nes.hh"
#include "../include/display.hh"
#include "../include/sound.hh"
#include "../include/ricoh_2a03.hh"
#include "../../include/memory.hh"
#include "../include/ppu.hh"
#include "../../include/apu_derived.hh"

using namespace std;

NES* VM = NULL;

enum InfoStatus {
    SUCCESS,
    ERROR,
    WARNING,
    NULL_PTR,
    INVALID_PARAM,
    READ_ERROR,
    WRITE_ERROR,
    
};

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

//* *********************
//*  iNES Class Methods
//* *********************
u8 NES::iNES_Read(const char* file_in) {
    //? Verify file can be opened
    ifstream NES_IN(file_in, ios::binary);
    if (NES_IN.fail()) {
        cout << "ERROR (in NES::iNES_Read): could not open file, \'" << file_in << "\', to be read as iNES. Continuing...\n";
        return READ_ERROR;
    }

    //? Initialize iNES FILE member object
    _DATA = new iNES(file_in);

    //* 32768 == $8000
    if (_DATA->file_size > 32768) {
        //? Greater than size of ROM page
        //? IDK how to handle this yet, so I just won't
        cout << "WARNING (in NES::iNES_Read): no support for >32KB files (yet). Continuing...\n";
        return READ_ERROR;
    }
    //? File will fit in ROM page
    else if (_DATA->file_size <= 32768 && _DATA->file_size > 0) {
        //? Store the file stream's data in the iNES object
        //? Also process the data and set appropriate flags and states for easy access
        u8 FILE_PROC_STATUS = _DATA->SaveData(NES_IN);
        return FILE_PROC_STATUS;
    }
    else if (_DATA->file_size == 0) {
        //? Empty file
        return READ_ERROR;
    }

    return SUCCESS;
}

u8 NES::iNES_Read(const char* file_in, uint16_t offset) {

    return SUCCESS;
}

u8 NES::iNES_Read(const char* file_in, uint16_t offset, uint16_t width) {

    return SUCCESS;
}

u8 NES::iNES_Write(const char* file_out,  vector<uint16_t> data, bool overwrite) {

    return SUCCESS;
}

u8 iNES::SaveData(ifstream& iNES_in) {
    //? Invalid file stream
    if (iNES_in.fail()) {
        cout << "ERROR (in iNES::SaveData): unable to access input iNES file stream. Exiting...\n";
        return READ_ERROR;
    }

    //? Get file size
    iNES_in.seekg(0, ios::end); // Go to end of file to get size
    file_size = (uint32_t)iNES_in.tellg();
    iNES_in.seekg(0, ios::beg); // Return to start of file

    //? Allocate buffer to store file data
    _DATA->file_data = new char[_DATA->file_size];

    //? Read file data into buffer, close the file
    iNES_in.read(_DATA->file_data, _DATA->file_size);
    iNES_in.close();

    //? Process file data
    u8 FILE_PROC_STATUS = ProcessRead();
    cout << "FILE \'" << filename << "\' PROCESSED w/ STATUS - " << FILE_PROC_STATUS << "\n";

    return SUCCESS;
}

u8 iNES::ProcessRead() {
    //? Fill in "header" (16 bytes)
    header = new vector<uint8_t>(file_data, file_data + 15);
    //* Set member flags based on header contents
    //* Bytes 0-3: ASCII "NES" followed by MSDOS EOF

    //* Byte 4: size of PRG_ROM, in 16KB units

    //* Byte 5: size of CHR_ROM, in 8KB units (0= use CHR_RAM)

    //* Byte 6: FLAGS 6; Mapper, mirroring, battery, trainer

    //* Byte 7: FLAGS 7; Mapper, VS/Playchoice, NES 2.0

    //* Byte 8: (rarely used) FLAGS 8; PRG-RAM size

    //* Byte 9: (rarely used) FLAGS 9; TV system

    //* Byte 10: (unofficial, rarely used) FLAGS 10; TV system, PRG-RAM presence

    //* Bytes 11-15: unused padding (can be zero, or leave secret message :D)
    //? Check for "trainer" (0 or 512 bytes)

    //? Fill in "PRG_ROM" (16384 * X bytes)

    //? Check for "CHR_ROM" (8192 * Y bytes)

    //? Check for "INSTR_ROM" (8192 bytes)
    
    //? Check for "PROM" (16 bytes Data, 16 bytes CounterOut)

    //? Check for "title" at end of file (127 or 128 bytes)


    return SUCCESS;
}

u8 iNES::ExportData(ofstream& iNES_out) {
    //TODO
    return SUCCESS;
}
