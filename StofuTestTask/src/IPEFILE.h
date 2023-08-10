#pragma once
#include "Struct.h"

class IPEFILE
{
public:
    IPEFILE() = default;
    virtual ~IPEFILE() = default;
private:
    virtual void Initialize() = 0;
public:
    virtual void UnloadFile() = 0;
    virtual void PrintImportTable() = 0;
    virtual PEResult LoadFromFile(int8_t* m_loadedPeFilePtr) = 0;
    virtual PEResult SaveToFile(std::filesystem::path filePath) = 0;

    virtual int32_t AddSection(std::string_view name, DWORD size, bool isExecutable) = 0;
    virtual void AddImport(std::string_view dllName, char** functions, int functionCount) = 0;
    virtual void Commit() = 0;

private:
    virtual PEResult IsValid() = 0;
    virtual PEResult ReadAll() = 0;
    virtual PEResult ReadHeaders() = 0;
    virtual void ReadSections() = 0;
    virtual PEResult ReadImports() = 0;


    virtual void RebuildImportTable() = 0;
    virtual char* BuildAdditionalImports(DWORD baseRVA) = 0;
    virtual DWORD CalculateAdditionalImportsSize(DWORD& sizeDlls, DWORD& sizeFunctions, DWORD& sizeStrings) = 0;

    virtual bool WritePadding(std::ofstream& file, long paddingSize) = 0;
    virtual DWORD AlignNumber(DWORD number, DWORD alignment) = 0;
    virtual DWORD RvaToOffset(DWORD rva) = 0;
    virtual DWORD OffsetToRVA(DWORD offset) = 0;

    virtual void ComputeReservedData() = 0;
    virtual void ComputeHeaders() = 0;
    virtual void ComputeSectionTable() = 0;
};