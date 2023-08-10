#pragma once

#include "IPEFILE.h"

class PE32FILE :public IPEFILE
{
public:
    PE32FILE();
    virtual ~PE32FILE();

    virtual void UnloadFile()override;
    virtual PEResult LoadFromFile(int8_t* m_loadedPeFilePtr)override;
    virtual PEResult SaveToFile(std::filesystem::path filePath)override;
    virtual int32_t AddSection(std::string_view name, DWORD size, bool isExecutable)override;
    virtual void AddImport(std::string_view dllName, char** functions, int functionCount)override;
    virtual void Commit()override;

    virtual void PrintImportTable()override;


private:
    IMAGE_DOS_HEADER m_dosHeader;
    PE_DOS_STUB m_dosStub;
    IMAGE_NT_HEADERS32 m_ntHeaders;
    IMAGE_SECTION_HEADER m_sectionTable[MAX_SECTIONS];
    PE_SECTION_ENTRY m_reservedData;
    PE_SECTION_ENTRY m_sections[MAX_SECTIONS];
    PE_IMPORT_DLL_ENTRY m_importTable;
    PE_IMPORT_DLL_ENTRY m_additionalImports;

    int8_t* m_loadedPeFile;

    virtual void Initialize()override;

    virtual PEResult IsValid()override;
    virtual PEResult ReadAll()override;
    virtual PEResult ReadHeaders()override;
    virtual void ReadSections()override;
    virtual PEResult ReadImports()override;

    virtual void RebuildImportTable()override;
    virtual char* BuildAdditionalImports(DWORD baseRVA)override;
    virtual DWORD CalculateAdditionalImportsSize(DWORD& sizeDlls, DWORD& sizeFunctions, DWORD& sizeStrings)override;

    virtual bool WritePadding(std::ofstream& file, long paddingSize)override;
    virtual DWORD AlignNumber(DWORD number, DWORD alignment)override;
    virtual DWORD RvaToOffset(DWORD rva)override;
    virtual DWORD OffsetToRVA(DWORD offset)override;

    void ComputeReservedData()override;
    virtual void ComputeHeaders()override;
    virtual void ComputeSectionTable()override;

};