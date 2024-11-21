#include <cstdint>
#include <iostream>
#include <fstream>

#include "../include/NES.hh"

using namespace std;

InfoStatus NES::iNES_Read(const char* file_in) {
    //? Verify file can be opened
    ifstream NES_IN(file_in, ios::binary);
    if (NES_IN.fail()) {
        cout << "ERROR (in NES::iNES_Read): could not open file, \'" << file_in << "\', to be read as iNES. Continuing...\n";
        return READ_ERROR;
    }

    //? Initialize iNES FILE member object
    FILE = new iNES(file_in);

    //* 32768 == $8000
    if (FILE->file_size > 32768) {
        //? Greater than size of ROM page
        //? IDK how to handle this yet, so I just won't
        cout << "WARNING (in NES::iNES_Read): no support for >32KB files (yet). Continuing...\n";
        return READ_ERROR;
    }
    //? File will fit in ROM page
    else if (FILE->file_size <= 32768 && FILE->file_size > 0) {
        //? Store the file stream's data in the iNES object
        //? Also process the data and set appropriate flags and states for easy access
        InfoStatus FILE_PROC_STATUS = FILE->SaveData(NES_IN);
        return FILE_PROC_STATUS;
    }
    else if (FILE->file_size == 0) {
        //? Empty file
        return READ_ERROR;
    }

    return SUCCESS;
}

InfoStatus NES::iNES_Read(const char* file_in, uint16_t offset) {

    return SUCCESS;
}

InfoStatus NES::iNES_Read(const char* file_in, uint16_t offset, uint16_t width) {

    return SUCCESS;
}

InfoStatus NES::iNES_Write(const char* file_out,  vector<uint16_t> data, bool overwrite) {

    return SUCCESS;
}

//* *********************
//*  iNES Class Methods
//* *********************
InfoStatus iNES::SaveData(ifstream& iNES_in) {
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
    FILE->file_data = new char[FILE->file_size];

    //? Read file data into buffer, close the file
    iNES_in.read(FILE->file_data, FILE->file_size);
    iNES_in.close();

    //? Process file data
    ist FILE_PROC_STATUS = ProcessRead();
    cout << "FILE \'" << filename << "\' PROCESSED w/ STATUS - " << IST_Strings[FILE_PROC_STATUS] << "\n";

    return SUCCESS;
}

InfoStatus iNES::ProcessRead() {
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

InfoStatus iNES::ExportData(ofstream& iNES_out) {
    //TODO
    return SUCCESS;
}
