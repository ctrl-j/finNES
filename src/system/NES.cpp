#include <iostream>
#include <cstdint>

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

int main(int argc, char** argv) {
    VM = new NES();
    
    delete VM;

    return EXIT_SUCCESS;
}

NES::NES() {
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
    int GUI_INIT_STATUS = _VID_OUT->Init();
    cout << "GUI INIT EXIT CODE: " << GUI_INIT_STATUS << "\n";

    //? Init vertex data for the starting screen
    vector<int> x_tri_1 = {200, 300, 500};
    vector<int> y_tri_1 = {200, 200, 100};

    vector<int> x_tri_2 = {400, 600, 500};
    vector<int> y_tri_2 = {200, 200, 100};

    vector<float> tri1 = _VID_OUT->PxToNDC_Tri(x_tri_1, y_tri_1);
    vector<float> tri2 = _VID_OUT->PxToNDC_Tri(x_tri_2, y_tri_2);

    _VID_OUT->verts = new float[18];

    int GUI_SCENE_STATUS = _VID_OUT->Scene();
    cout << "GUI SCENE EXIT CODE: " << GUI_SCENE_STATUS << "\n";

    delete _VID_OUT->verts;
    
    //? Start the virtual machine

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