#pragma once
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

constexpr auto MAX_SECTIONS = 64;
constexpr auto IMPORT_SECTION_NAME = "@.import";
constexpr auto RESERV_SECTION_NAME = "@.reserv";

enum class PEResult : uint32_t
{
    Success = 0,

    ErrorReadFileOpen = 1,
    ErrorReadFileAlloc = 2,
    ErrorReadFileData = 3,

    ErrorInvalidFileSignature = 10,
    ErrorInvalidFileTooManySections = 11,

    ErrorReadImportsTableOffset = 20,

    ErrorAddSectionMaxReached = 30,

    ErrorSaveFileCreate = 40,

    ErrorCheckMashine = 50

};

typedef struct PE_DOS_STUB
{
    int8_t* RawData;
    DWORD Size;
};

typedef struct PE_IMPORT_FUNCTION_ENTRY
{
    char* Name;
    int Id;
    PE_IMPORT_FUNCTION_ENTRY* Next;
};

typedef struct PE_IMPORT_DLL_ENTRY
{
    char* Name;
    PE_IMPORT_FUNCTION_ENTRY* Functions;
    PE_IMPORT_DLL_ENTRY* Next;
};

typedef struct PE_SECTION_ENTRY
{
    DWORD Offset;
    int8_t* RawData;
    DWORD Size;
};
