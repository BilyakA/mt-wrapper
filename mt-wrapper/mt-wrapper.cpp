#include <windows.h>
#include <stdio.h>
#include <process.h>

BOOL FileExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int __cdecl wmain(int argc, WCHAR **argv, WCHAR **env)
{
    int tries = 0;
    
    // Check if we can find mt-orig.exe
    if (!FileExists("mt-orig.exe"))
    {
        fprintf(stderr, "Error: mt-orig.exe not found!\n\n");
        return 1;
    }
    // Get it full path and pass to argv[0]
    wchar_t* path = _wfullpath(NULL, L"mt-orig.exe", 1024);
    argv[0] = path;
    // Run the original mt.exe, which has been renamed to mt-orig.exe.
    for (;;)
    {
        // Try to run the original mt.
        intptr_t iStatus = _wspawnve(_P_WAIT, path, argv, env);
        if (iStatus == 0)
            break;
        // Just some info message about how usefull we are
        fprintf(stdout, "Info: mt.exe failed! Retrying...\n\n");
        
        if (++tries > 10) {
            // In case something went VERY wrong...
            fprintf(stderr, "Failed: mt.exe failed 10 times! Giving up.\n\n");
            return 2;
        }
        // Try again, after a short wait.
        ::Sleep(100);
    }

    return 0;
}