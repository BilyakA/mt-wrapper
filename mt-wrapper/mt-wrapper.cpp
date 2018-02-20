#include <windows.h>
#include <stdio.h>
#include <process.h>

BOOL FileExists(LPCWSTR szPath)
{
    DWORD dwAttrib = GetFileAttributesW(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int __cdecl wmain(int argc, WCHAR **argv, WCHAR **env)
{
    int tries = 0;
    
    // Get absolute path of wrapper
    WCHAR path[1024] = { '\0' };
    WCHAR origpath[2014] = { '\0' };
    GetModuleFileNameW(NULL, origpath, MAX_PATH + 1);
    wcsncat_s(path, origpath, wcsrchr(origpath, '\\') - origpath);
    wcscat_s(path, L"\\mt-orig.exe");

    // Check if we can find mt-orig.exe
    if (!FileExists(path))
    {
        fprintf(stderr, "Error: mt-orig.exe not found!\n\n");
        return 1;
    }
    // Escape full path with qoutes and pass to argv[0]
    WCHAR argv0[1024] = {'\0'};
    wcscpy_s(argv0, L"\"");
    wcscat_s(argv0, path);
    wcscat_s(argv0, L"\"");
    argv[0] = argv0;
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