#include "PE32FILE.h"
#include "PE64FILE.h"

int WaitForExit(PEResult rc, const char* msg)
{
    if (rc != PEResult::Success)
        std::cout << msg << (int)rc << std::endl;
    else
        std::cout << msg << std::endl;

    std::cout << "Press ENTER to exit.." << std::endl;
    std::cin.ignore();

    return (int)rc;
}

int8_t* ReadFile(std::filesystem::path filePath)
{
    int8_t* m_loadedPeFile;

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    auto fileSize = file.tellg();

    file.seekg(0);

    m_loadedPeFile = (int8_t*)VirtualAlloc(NULL, fileSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    file.read((char*)m_loadedPeFile, fileSize);

    file.close();

    return m_loadedPeFile;
}

int CheckMachine(int8_t* m_loadedPeFile) {
    // Assuming m_loadedPeFile points to the beginning of the loaded PE file
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)m_loadedPeFile;
    if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE)
    {
        IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)((uint8_t*)m_loadedPeFile + dosHeader->e_lfanew);
        if (ntHeader->Signature == IMAGE_NT_SIGNATURE)
        {
            if (ntHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64)
            {
                return 1;
            }
            else if (ntHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
            {
                return 0;
            }
            else
            {
                return 100;
            }
        }
    }

}

int start(std::unique_ptr<IPEFILE>& peFILE, int8_t* m_loadedPeFile, const char* nameDll) {


    PEResult rc = peFILE->LoadFromFile(m_loadedPeFile);
    if (rc != PEResult::Success)
    {
        return WaitForExit(rc, "Failed to load file: ");
    }

    // Print import table
    peFILE->PrintImportTable();

    // Add the exported functions of your DLL
    const char* functions[] = { "MyFunction" };

    // Add the import to the PE file
    peFILE->AddImport(nameDll, (char**)functions, 1);

    // Save the modified file
    rc = peFILE->SaveToFile("new.exe");
    if (rc != PEResult::Success)
    {
        return WaitForExit(rc, "Failed to save file: ");
    }

};

int main(int argc, char* argv[])
{
    //const char* filePath = "new.exe";
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <PEFilePath>" << std::endl;
        std::cout << "Press ENTER to exit.." << std::endl;
        std::cin.ignore();
        std::cin.get();
        return 1; // Return error code
    }

    const char* peFilePath = argv[1];

    const char* nameDll = argv[2];


    std::unique_ptr<IPEFILE> peFILE;

    int8_t* m_loadedPeFile = ReadFile(peFilePath);

    int machine = CheckMachine(m_loadedPeFile);

    if (machine == 0)
    {
        peFILE = std::make_unique<PE32FILE>();
        start(peFILE, m_loadedPeFile, nameDll);
    }
    else if (machine == 1) {
        peFILE = std::make_unique<PE64FILE>();
        start(peFILE, m_loadedPeFile, nameDll);
    }
    else {
        WaitForExit(PEResult::ErrorCheckMashine, "Error check machine");
    }
    return WaitForExit(PEResult::Success, "Saved modified PE file");

}