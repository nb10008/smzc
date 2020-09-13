
//
// Dont use Precompiled Headers 
//

#include <windows.h>
#include <Wintrust.h>
#include <softpub.h>
#include "Mscat.h"
#include "SignVerify.h"

#pragma comment(lib, "Wintrust.lib")


#define MyMalloc(sz) (unsigned char *)malloc(sz)
#define MyFree(handle) free(handle) 
#define SIZECHARS(x) (sizeof((x))/sizeof(TCHAR))
//
// DLL array structures
//

#define MAX_DLL_ARRAY   5



typedef struct {
    PCSTR DllName;
    HINSTANCE DllInst;
} DLLTABLE, *PDLLTABLE;

static INT g_ArraySize = 0;
static DLLTABLE g_DllArray[MAX_DLL_ARRAY];
FARPROC
ObtainFnPtr (
    IN      PCSTR DllName,
    IN      PCSTR ProcName,
    IN      FARPROC Default
    );

extern "C"{
typedef BOOL (WINAPI *CRYPTCATADMINACQUIRECONTEXT)(
            OUT HCATADMIN *phCatAdmin,
            IN const GUID *pgSubsystem,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CRYPTCATADMINRELEASECONTEXT)(
            IN HCATADMIN hCatAdmin,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CRYPTCATADMINRELEASECATALOGCONTEXT)(
            IN HCATADMIN hCatAdmin,
            IN HCATINFO hCatInfo,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CRYPTCATCATALOGINFOFROMCONTEXT)(
            IN HCATINFO hCatInfo,
            IN OUT CATALOG_INFO *psCatInfo,
            IN DWORD dwFlags
            );

typedef BOOL (WINAPI *CRYPTCATADMINCALCHASHFROMFILEHANDLE)(
            IN HANDLE hFile,
            IN OUT DWORD *pcbHash,
            OUT OPTIONAL BYTE *pbHash,
            IN DWORD dwFlags
            );

typedef HCATINFO (WINAPI *CRYPTCATADMINENUMCATALOGFROMHASH)(
            IN HCATADMIN hCatAdmin,
            IN BYTE *pbHash,
            IN DWORD cbHash,
            IN DWORD dwFlags,
            IN OUT HCATINFO *phPrevCatInfo
            );

typedef LONG (WINAPI *WINVERIFYTRUST)(
            HWND hwnd,
            GUID *pgActionID,
            LPVOID pWVTData
            );


CRYPTCATADMINACQUIRECONTEXT Dyn_CryptCATAdminAcquireContext = NULL;
CRYPTCATADMINRELEASECONTEXT Dyn_CryptCATAdminReleaseContext = NULL;
CRYPTCATADMINRELEASECATALOGCONTEXT Dyn_CryptCATAdminReleaseCatalogContext = NULL;
CRYPTCATCATALOGINFOFROMCONTEXT Dyn_CryptCATCatalogInfoFromContext = NULL;
CRYPTCATADMINCALCHASHFROMFILEHANDLE Dyn_CryptCATAdminCalcHashFromFileHandle = NULL;
CRYPTCATADMINENUMCATALOGFROMHASH Dyn_CryptCATAdminEnumCatalogFromHash = NULL;
WINVERIFYTRUST Dyn_WinVerifyTrust = NULL;

}
//#endif

int initSignVerify();


GUID DriverVerifyGuid = DRIVER_ACTION_VERIFY;
GUID WintrustVerifyGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;

PCTSTR
MyGetFileTitle(
    IN PCTSTR FilePath
    )

/*++

Routine Description:

    This routine returns a pointer to the first character in the
    filename part of the supplied path.  If only a filename was given,
    then this will be a pointer to the first character in the string
    (i.e., the same as what was passed in).

    To find the filename part, the routine returns the last component of
    the string, beginning with the character immediately following the
    last '\', '/' or ':'. (NB NT treats '/' as equivalent to '\' )

Arguments:

    FilePath - Supplies the file path from which to retrieve the filename
        portion.

Return Value:

    A pointer to the beginning of the filename portion of the path.

--*/

{
    PCTSTR LastComponent = FilePath;
    TCHAR  CurChar;

    while(CurChar = *FilePath) {
        FilePath = CharNext(FilePath);
        if((CurChar == TEXT('\\')) || (CurChar == TEXT('/')) || (CurChar == TEXT(':'))) {
            LastComponent = FilePath;
        }
    }

    return LastComponent;
}
DWORD
_VerifyFile(
    IN  LPCTSTR                Key,
    IN  LPCTSTR                FileFullPath
    )

/*++

Routine Description:

    This routine verifies a single file against a particular catalog file, or
    against any installed catalog file.

Arguments:

    Key - supplies a value that "indexes" the catalog, telling the verify APIs
        which signature datum within the catalog it should use. Typically
        the key is the (original) filename of a file.

    FileFullPath - supplies the full path of the file to be verified.
Return Value:

    If successful, the return value is NO_ERROR.
    If failure, the return value is a Win32 error code indicating the cause of
    the failure.

--*/

{
    LPBYTE Hash;
    DWORD HashSize;
    CATALOG_INFO CatInfo;
    HANDLE hFile;
    HCATADMIN hCatAdmin;
    HCATINFO hCatInfo;
    HCATINFO PrevCat;
    DWORD Err;
    WINTRUST_DATA WintrustData;
    WINTRUST_CATALOG_INFO WintrustCatalogInfo;
    WINTRUST_FILE_INFO WintrustFileInfo;
    LPTSTR CatalogFullPath;
    WCHAR UnicodeKey[MAX_PATH];
#ifndef UNICODE
    CHAR AnsiBuffer[MAX_PATH];
#endif

#ifndef NODYNAPI
    if(!initSignVerify())
    {
        return -1;
    }
#endif

    if(
#ifdef NODYNAPI
        CryptCATAdminAcquireContext(&hCatAdmin, &DriverVerifyGuid, 0)
#else
        Dyn_CryptCATAdminAcquireContext(&hCatAdmin, &DriverVerifyGuid, 0)
#endif
        ) {

        hFile = CreateFile(FileFullPath,
                           GENERIC_READ,
                           FILE_SHARE_READ | FILE_SHARE_WRITE,
                           NULL,
                           OPEN_EXISTING,
                           0,
                           NULL
                          );

        if(hFile == INVALID_HANDLE_VALUE) {
            Err = GetLastError();
        } else {
            //
            // Initialize some of the structures that will be used later on
            // in calls to WinVerifyTrust.  We don't know if we're verifying
            // against a catalog or against a file yet.
            //
            ZeroMemory(&WintrustData, sizeof(WINTRUST_DATA));
            WintrustData.cbStruct = sizeof(WINTRUST_DATA);
            WintrustData.dwUIChoice = WTD_UI_NONE;
            WintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
            WintrustData.dwStateAction = WTD_STATEACTION_AUTO_CACHE;
            WintrustData.dwProvFlags = WTD_REVOCATION_CHECK_NONE;

            //
            // Start out with a hash buffer size that should be large enough for
            // most requests.
            //
            HashSize = 100;
            do {
                Hash = MyMalloc(HashSize);
                if(!Hash) {
                    Err = ERROR_NOT_ENOUGH_MEMORY;
                    break;
                }
                if(
#ifdef NODYNAPI
                    CryptCATAdminCalcHashFromFileHandle(hFile, &HashSize, Hash, 0)
#else
                    Dyn_CryptCATAdminCalcHashFromFileHandle(hFile, &HashSize, Hash, 0)
#endif
                ) {
                    Err = NO_ERROR;
                } else {
                    Err = GetLastError();
                    //
                    // If this API did screw up and not set last error, go ahead
                    // and set something.
                    //
                    if(Err == NO_ERROR) {
                        Err = ERROR_INVALID_DATA;
                    }

                    MyFree(Hash);
                    if(Err != ERROR_INSUFFICIENT_BUFFER) {
                        //
                        // The API failed for some reason other than
                        // buffer-too-small.  We will try to check if the file
                        // is self-signed.
                        //
                        //
                        Hash = NULL;  // reset this so we won't try to free it
                                      //  later
                        hCatInfo = NULL;
                        CloseHandle(hFile);

                        //
                        // set the last error code here so that it's set
                        // appropriately when we get to the selfsign block.
                        //
                        SetLastError(Err);

                        goto selfsign;
                    }
                }
            } while(Err != NO_ERROR);

            CloseHandle(hFile);

            if(Err == NO_ERROR) {
                //
                // Now we have the file's hash.  Initialize the structures that
                // will be used later on in calls to WinVerifyTrust.
                //
                WintrustData.dwUnionChoice = WTD_CHOICE_CATALOG;
                WintrustData.pCatalog = &WintrustCatalogInfo;

                ZeroMemory(&WintrustCatalogInfo, sizeof(WINTRUST_CATALOG_INFO));
                WintrustCatalogInfo.cbStruct = sizeof(WINTRUST_CATALOG_INFO);
                WintrustCatalogInfo.pbCalculatedFileHash = Hash;
                WintrustCatalogInfo.cbCalculatedFileHash = HashSize;

                //
                // WinVerifyTrust is case-sensitive, so ensure that the key
                // being used is all lower-case!
                //
#ifdef UNICODE
                //
                // Copy the key to a writable Unicode character buffer so we
                // can lower-case it.
                //
                lstrcpyn(UnicodeKey, Key, SIZECHARS(UnicodeKey));
                CharLower(UnicodeKey);
#else
                //
                // Copy the key to a writable ANSI character buffer so we can
                // lower-case it (prior to converting the string to Unicode).
                //
                lstrcpyn(AnsiBuffer, Key, SIZECHARS(AnsiBuffer));
                CharLower(AnsiBuffer);
                MultiByteToWideChar(CP_ACP, 0, AnsiBuffer, -1, UnicodeKey, SIZECHARS(UnicodeKey));
#endif
                WintrustCatalogInfo.pcwszMemberTag = UnicodeKey;

                    //
                    // Search through installed catalogs looking for those that
                    // contain data for a file with the hash we just calculated.
                    //
                    PrevCat = NULL;
#ifdef NODYNAPI
                    hCatInfo = CryptCATAdminEnumCatalogFromHash(hCatAdmin,
                                                                Hash,
                                                                HashSize,
                                                                0,
                                                                &PrevCat
                                                               );
#else
                    hCatInfo = Dyn_CryptCATAdminEnumCatalogFromHash(hCatAdmin,
                                                                Hash,
                                                                HashSize,
                                                                0,
                                                                &PrevCat
                                                               );
#endif


                    while(hCatInfo) {

                        CatInfo.cbStruct = sizeof(CATALOG_INFO);
                        if(
#ifdef NODYNAPI
                            CryptCATCatalogInfoFromContext(hCatInfo, &CatInfo, 0)
#else
                            Dyn_CryptCATCatalogInfoFromContext(hCatInfo, &CatInfo, 0)
#endif
                            ) {
#ifdef UNICODE
                            CatalogFullPath = CatInfo.wszCatalogFile;
#else
                            WideCharToMultiByte(
                                CP_ACP,
                                0,
                                CatInfo.wszCatalogFile,
                                -1,
                                AnsiBuffer,
                                sizeof(AnsiBuffer),
                                NULL,
                                NULL
                                );
                            CatalogFullPath = AnsiBuffer;
#endif
                            //////////////////////////////////////////////////
                            //
                            // If we have a catalog name we're looking for,
                            // see if the current catalog matches.  If the
                            // caller didn't specify a catalog, then just
                            // attempt to validate against each catalog we
                            // enumerate.  Note that the catalog file info we
                            // get back gives us a fully qualified path.
                            //

                            //
                            // We found an applicable catalog, now
                            // validate the file against that catalog.
                            //
                            // NOTE:  Because we're using cached
                            // catalog information (i.e., the
                            // WTD_STATEACTION_AUTO_CACHE flag), we
                            // don't need to explicitly validate the
                            // catalog itself first.
                            //
                            WintrustCatalogInfo.pcwszCatalogFilePath = CatInfo.wszCatalogFile;
#ifdef NODYNAPI
                            Err = (DWORD)WinVerifyTrust(NULL,
                                &DriverVerifyGuid,
                                &WintrustData
                                );
#else
                            Err = (DWORD)Dyn_WinVerifyTrust(NULL,
                                &DriverVerifyGuid,
                                &WintrustData
                                );
#endif

                            //
                            // If the result of the above validations is
                            // success, then we're done.  If not, and we're
                            // looking for a relevant catalog file (i.e.,
                            // the INF didn't specify one), then we move
                            // on to the next catalog.  Otherwise, we've
                            // failed.
                            //
                            if((Err == NO_ERROR)) {
#ifdef NODYNAPI
                                CryptCATAdminReleaseCatalogContext(hCatAdmin, hCatInfo, 0);
#else
                                Dyn_CryptCATAdminReleaseCatalogContext(hCatAdmin, hCatInfo, 0);
#endif
                                break;
                            }
                            //////////////////////////////////////////////////

                        }


                        PrevCat = hCatInfo;
#ifdef NODYNAPI
                        hCatInfo = CryptCATAdminEnumCatalogFromHash(hCatAdmin, Hash, HashSize, 0, &PrevCat);
#else
                        hCatInfo = Dyn_CryptCATAdminEnumCatalogFromHash(hCatAdmin, Hash, HashSize, 0, &PrevCat);
#endif
                    }
selfsign:
                    if(!hCatInfo) {
                        //
                        // We exhausted all the applicable catalogs without
                        // finding the one we needed.
                        //
                        Err = GetLastError();
                        //
                        // Make sure we have a valid error code.
                        //
                        if(Err == NO_ERROR) {
                            Err = ERROR_INVALID_DATA;
                        }

                        //
                        // The file failed to validate using the specified
                        // catalog.  See if the file validates without a
                        // catalog (i.e., the file contains its own
                        // signature).
                        //
                        WintrustData.dwUnionChoice = WTD_CHOICE_FILE;
                        WintrustData.pFile = &WintrustFileInfo;
                        ZeroMemory(&WintrustFileInfo, sizeof(WINTRUST_FILE_INFO));
                        WintrustFileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
#ifdef UNICODE
                        WintrustFileInfo.pcwszFilePath = FileFullPath;
#else
                        //
                        // Use the UnicodeKey buffer to hold the unicode
                        // version of the full pathname of the file to be
                        // verified.
                        //
                        MultiByteToWideChar(CP_ACP, 0, FileFullPath, -1, UnicodeKey, SIZECHARS(UnicodeKey));
                        WintrustFileInfo.pcwszFilePath = UnicodeKey;
#endif
#ifdef NODYNAPI
                        Err = (DWORD)WinVerifyTrust(NULL,
                                                    &DriverVerifyGuid,
                                                    &WintrustData
                                                    );
#else
                        Err = (DWORD)Dyn_WinVerifyTrust(NULL,
                                                    &DriverVerifyGuid,
                                                    &WintrustData
                                                    );
#endif
                        if(Err)
                        {
#ifdef NODYNAPI
                            Err = (DWORD)WinVerifyTrust(NULL,
                                                    &WintrustVerifyGuid,
                                                    &WintrustData
                                                   );
#else
                            Err = (DWORD)Dyn_WinVerifyTrust(NULL,
                                                    &WintrustVerifyGuid,
                                                    &WintrustData
                                                   );
#endif
                        } 
                    }
            } 
            if(Hash) {
                MyFree(Hash);
            }
        }
#ifdef NODYNAPI
        CryptCATAdminReleaseContext(hCatAdmin,0);
#else
        Dyn_CryptCATAdminReleaseContext(hCatAdmin,0);
#endif
    } else {
        Err = GetLastError();
        //
        // Make sure we have a valid error code.
        //
        if(Err == NO_ERROR) {
            Err = ERROR_INVALID_DATA;
        }
    }

    SetLastError(Err);
    return Err;
}

DWORD VerifySignFile(
    IN  LPCTSTR                FileFullPath
    )
{
    return _VerifyFile(
        MyGetFileTitle(FileFullPath),
        FileFullPath
        );
}

FARPROC
ObtainFnPtr (
    IN      PCSTR DllName,
    IN      PCSTR ProcName,
    IN      FARPROC Default
    )

/*++

Routine Description:

    This routine manages an array of DLL instance handles and returns the
    proc address of the caller-specified routine.  The DLL is loaded
    and remains loaded until the DLL terminates.  This array is not
    synchronized.

Arguments:

    DllName - The ANSI DLL name to load

    ProcName - The ANSI procedure name to locate

    Default - The default procedure, if the export was not found

Return Value:

    The address of the requested function, or NULL if the DLL could not
    be loaded, or the function is not implemented in the loaded DLL.

--*/

{
    INT i;
    PSTR DupBuf;
    FARPROC Address = NULL;

    //
    // Search for loaded DLL
    //

    for (i = 0 ; i < g_ArraySize ; i++) {
        if (!lstrcmpiA (DllName, g_DllArray[i].DllName)) {
            break;
        }
    }

    do {
        //
        // If necessary, load the DLL
        //

        if (i == g_ArraySize) {
            if (g_ArraySize == MAX_DLL_ARRAY) {
                // Constant limit needs to be raised
                break;
            }

            g_DllArray[i].DllInst = LoadLibraryA (DllName);
            if (!g_DllArray[i].DllInst) {
                break;
            }

            DupBuf = (PSTR) MyMalloc (lstrlenA (DllName) + 1);
            if (!DupBuf) {
                break;
            }
            lstrcpyA (DupBuf, DllName);
            g_DllArray[i].DllName = DupBuf;

            g_ArraySize++;
        }

        //
        // Now that DLL is loaded, return the proc address if it exists
        //

        Address = GetProcAddress (g_DllArray[i].DllInst, ProcName);

    } while (FALSE);

    if (!Address) {
        return Default;
    }

    return Address;
}

int initSignVerify()
{
    int nResult = 0;
    //
    if(NULL == Dyn_CryptCATAdminAcquireContext)
        Dyn_CryptCATAdminAcquireContext = (CRYPTCATADMINACQUIRECONTEXT)ObtainFnPtr(
            "wintrust.dll","CryptCATAdminAcquireContext",
            (FARPROC) NULL
        );
    if(NULL == Dyn_CryptCATAdminAcquireContext)
        goto Exit0;

    if(NULL == Dyn_CryptCATAdminReleaseContext)
        Dyn_CryptCATAdminReleaseContext = (CRYPTCATADMINRELEASECONTEXT)ObtainFnPtr (
            "wintrust.dll",
            "CryptCATAdminReleaseContext",
            (FARPROC) NULL
        );
    if(NULL == Dyn_CryptCATAdminReleaseContext)
        goto Exit0;

    if(NULL == Dyn_CryptCATAdminReleaseCatalogContext)
        Dyn_CryptCATAdminReleaseCatalogContext = (CRYPTCATADMINRELEASECATALOGCONTEXT)ObtainFnPtr (
            "wintrust.dll",
            "CryptCATAdminReleaseCatalogContext",
            (FARPROC) NULL
        );
    if(NULL == Dyn_CryptCATAdminReleaseCatalogContext)
        goto Exit0;

    if(NULL == Dyn_CryptCATCatalogInfoFromContext)
        Dyn_CryptCATCatalogInfoFromContext = (CRYPTCATCATALOGINFOFROMCONTEXT)ObtainFnPtr (
            "wintrust.dll",
            "CryptCATCatalogInfoFromContext",
            (FARPROC) NULL
        );
    if(NULL == Dyn_CryptCATCatalogInfoFromContext)
        goto Exit0;

    if(NULL == Dyn_CryptCATAdminCalcHashFromFileHandle)
        Dyn_CryptCATAdminCalcHashFromFileHandle =(CRYPTCATADMINCALCHASHFROMFILEHANDLE)ObtainFnPtr (
            "wintrust.dll",
            "CryptCATAdminCalcHashFromFileHandle",
            (FARPROC) NULL
        );
    if(NULL == Dyn_CryptCATAdminCalcHashFromFileHandle)
        goto Exit0;

    if(NULL == Dyn_CryptCATAdminEnumCatalogFromHash)
        Dyn_CryptCATAdminEnumCatalogFromHash = (CRYPTCATADMINENUMCATALOGFROMHASH)ObtainFnPtr (
            "wintrust.dll",
            "CryptCATAdminEnumCatalogFromHash",
            (FARPROC) NULL
        );
    if(NULL == Dyn_CryptCATAdminEnumCatalogFromHash)
        goto Exit0;

    if(NULL == Dyn_WinVerifyTrust)
        Dyn_WinVerifyTrust = (WINVERIFYTRUST)ObtainFnPtr (
            "wintrust.dll",
            "WinVerifyTrust",
            (FARPROC) NULL
        );
    if(NULL == Dyn_WinVerifyTrust)
        goto Exit0;
    nResult = 1;
Exit0:
    return nResult;
}
