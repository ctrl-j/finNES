#include <cstdint>
#include <iostream>
#include <fstream>

#include "../../include/nes.hh"

using namespace std;

InfoStatus NES::iNES_Read(const char* _DATA_in) {
    //? Verify _DATA can be opened
    ifstream NES_IN(_DATA_in, ios::binary);
    if (NES_IN.fail()) {
        cout << "ERROR (in NES::iNES_Read): could not open _DATA, \'" << _DATA_in << "\', to be read as iNES. Continuing...\n";
        return READ_ERROR;
    }

    //? Initialize iNES _DATA member object
    _DATA = new iNES(_DATA_in);

    //* 32768 == $8000
    if (_DATA->file_size > 32768) {
        //? Greater than size of ROM page
        //? IDK how to handle this yet, so I just won't
        cout << "WARNING (in NES::iNES_Read): no support for >32KB _DATAs (yet). Continuing...\n";
        return READ_ERROR;
    }
    //? _DATA will fit in ROM page
    else if (_DATA->file_size <= 32768 && _DATA->file_size > 0) {
        //? Store the _DATA stream's data in the iNES object
        //? Also process the data and set appropriate flags and states for easy access
        InfoStatus _DATA_PROC_STATUS = _DATA->SaveData(NES_IN);
        return _DATA_PROC_STATUS;
    }
    else if (_DATA->file_size == 0) {
        //? Empty _DATA
        return READ_ERROR;
    }

    return SUCCESS;
}

InfoStatus NES::iNES_Read(const char* _DATA_in, uint16_t offset) {

    return SUCCESS;
}

InfoStatus NES::iNES_Read(const char* _DATA_in, uint16_t offset, uint16_t width) {

    return SUCCESS;
}

InfoStatus NES::iNES_Write(const char* _DATA_out,  vector<uint16_t> data, bool overwrite) {

    return SUCCESS;
}

//* *********************
//*  iNES Class Methods
//* *********************
InfoStatus iNES::SaveData(ifstream& iNES_in) {
    //? Invalid _DATA stream
    if (iNES_in.fail()) {
        cout << "ERROR (in iNES::SaveData): unable to access input iNES _DATA stream. Exiting...\n";
        return READ_ERROR;
    }

    //? Get _DATA size
    iNES_in.seekg(0, ios::end); // Go to end of _DATA to get size
    _DATA->file_size = (uint32_t)iNES_in.tellg();
    iNES_in.seekg(0, ios::beg); // Return to start of _DATA

    //? Allocate buffer to store _DATA data
    _DATA->file_data = new char[_DATA->file_size];

    //? Read _DATA data into buffer, close the _DATA
    iNES_in.read(_DATA->file_data, _DATA->file_size);
    iNES_in.close();

    //? Process _DATA data
    ist _DATA_PROC_STATUS = ProcessRead();
    cout << "FILE \'" << filename << "\' PROCESSED w/ STATUS - " << IST_Strings[_DATA_PROC_STATUS] << "\n";

    return SUCCESS;
}

InfoStatus iNES::ProcessRead() {
    //? Fill in "header" (16 bytes)
    header = new vector<uint8_t>(file_size, file_size + 15);
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

    //? Check for "title" at end of _DATA (127 or 128 bytes)


    return SUCCESS;
}

InfoStatus iNES::ExportData(ofstream& iNES_out) {
    //TODO
    return SUCCESS;
}
