/* Flash OS Routines (Automagically Generated)
 * Copyright (c) 2009-2015 ARM Limited
 */
#include "flash_blob.h"

static const uint32_t flash_code[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0xF2400A88, 0x43484132, 0x31FFF04F, 0x5110EB01, 0x65014834, 0x60012101, 0x47702000, 0x47702000,
    0x21014831, 0x21006141, 0x4A306001, 0x68816082, 0xD4FC0749, 0x3100F44F, 0x60826001, 0x07496881,
    0x68C0D4FC, 0xD00007C0, 0x47702001, 0xD10C0581, 0x22014925, 0x6008614A, 0x1E804824, 0x68886088,
    0xD4FC0780, 0x07C068C8, 0x2001D000, 0xB5704770, 0xD1350783, 0x4B1C1CC9, 0xF0212401, 0x615C0103,
    0x34B04C18, 0x07E56824, 0xD1174C19, 0xF020E023, 0x601D057F, 0xF000E007, 0xCA20067C, 0xF8C6441E,
    0x1D005100, 0xF0101F09, 0xD1010F7C, 0xB90D6B1D, 0xD1F02900, 0x6A1D621C, 0xD1FC07ED, 0xD1E62900,
    0x6018E00B, 0x605D6815, 0x689D609C, 0xD1FC07ED, 0x1D121D00, 0x29001F09, 0x68D8D1F3, 0xD00007C0,
    0xBD702001, 0x400FE0F0, 0x400FD000, 0xA4420004, 0xA4420001, 0x00000000
};

const program_target_t flash_algo = {
    0x20000021,  // Init
    0x2000003D,  // UnInit
    0x20000041,  // EraseChip
    0x2000006D,  // EraseSector
    0x2000008F,  // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : address to access global/static data
    // RSP  : stack pointer
    {
        0x20000001,
        0x20000C00,
        0x20001000
    },

    0x20000400,  // mem buffer location
    0x20000000,  // location to write prog_blob in target RAM
    sizeof(flash_code),  // prog_blob size
    flash_code,  // address of prog_blob
    0x00000400,  // ram_to_flash_bytes_to_be_written
};
