/*
 * [1] Mount EFI
 * [2] Replace bootmgfw.efi
 * [3] Profit
 */

#include <stdio.h>
#include <Windows.h>

#ifdef _LAZER_MD5_CHECK
#include <openssl/md5.h>

#define MD5_LEN_MAX 32

/* Create MD5sum of a file. Return value is a char pointer which needs to be free'd. */
char* MD5SUM_INIT(FILE* fp) {
    if (!fp) { return NULL; }

    int n;
    MD5_CTX c;
    long bytes;
    char tmpbuf[512];
    unsigned char digout[MD5_DIGEST_LENGTH];

    char* md5str = malloc(sizeof(char*) * (MD5_LEN_MAX + 1));
    if (!md5str) {
        return NULL;
    }

    MD5_Init(&c);

    do
    {
        bytes = (long)fread(tmpbuf, 1, 512, fp);
        MD5_Update(&c, tmpbuf, (size_t)bytes);
    } while (bytes > 0);

    MD5_Final(digout, &c);
    for (n = 0; n < MD5_DIGEST_LENGTH; n++) {
        sprintf(&md5str[n * 2], "%02x", (unsigned int)digout[n]);
    }

    return md5str;
}

/* Finalize (free) md5sum string */
void MD5SUM_FINAL(char* md5sum_str) {
    if (md5sum_str != NULL) {
        free(md5sum_str);
    }
}
#endif

int promptUnsafe(void) {
    printf("***************************\n***     ! WARNING !     ***\n***************************\nThe program is going to execute 'system(\"mountvol Q: /S\")' to mount and ! ALTER ! the EFI SYSTEM Partition, because it's convenient.. LOL\nThis software is licensed under the Apache License 2.0\nAre you okay with this? [y/N]: ");
	fflush(stdout);

	unsigned char prompt = fgetc(stdin);
	if (prompt == 'y' || prompt == 'Y') {
		printf("\nWhatever, it's your computer...\n");
        Sleep(2000);
		return EXIT_SUCCESS;
	}

	printf("Good thinking..\n");
    return EXIT_FAILURE;
}

BOOL FileExists(const char* szPath) {
    DWORD dwAttrib = GetFileAttributesA(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL DirectoryExists(const char* szPath)
{
    DWORD dwAttrib = GetFileAttributesA(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void cls(HANDLE hConsole)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}

char* GetErrorMessage(DWORD dwErrorCode, LPSTR pBuffer, DWORD cchBufferLength) {
    char* ptr = NULL;
    char buffer[4096];
    if (pBuffer == NULL) {
        ptr = buffer;
        cchBufferLength = 4095;
    }
    else {
        ptr = pBuffer;
    }
    if (cchBufferLength == 0) { return NULL; }

    DWORD cchMsg = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), ptr, cchBufferLength, NULL);
    return ptr;
}

int esp_install(void) {
    const char* efi_rwlazer_dir = "Q:\\EFI\\rwlazer";
    const char* efi_rwlazer_efi = "Q:\\EFI\\rwlazer\\rwlazer.efi";
    const char *efi_bootefi = "Q:\\EFI\\Boot\\bootx64.efi";
    const char* win_backupefi = "Q:\\EFI\\Boot\\winbootx64.efi";

    const char* lazer_bootefi = efi_bootefi + 3;   
    const char* lazer_lazerefi = efi_rwlazer_efi + 3;

    /* Create RWLAZER dir in ESP */
    if (!DirectoryExists(efi_rwlazer_dir)) {
        if (!CreateDirectoryA(efi_rwlazer_dir, NULL)) {
            fprintf(stderr, "[-] Unable to create RWLAZER directory in EFI System Partition. - %s\n", GetErrorMessage(GetLastError(), NULL, 0xfff));
            return EXIT_FAILURE;
        }
    }

    /* Backup BOOTX64.EFI */
    if (!MoveFileA(efi_bootefi, win_backupefi)) {
        fprintf(stderr, "[-] Unable to backup 'bootx64.efi' - %s\n", GetErrorMessage(GetLastError(), NULL, 0xfff));
        return EXIT_FAILURE;
    }

    /* Replace BOOTX64 with BGRT Injector */
    if (!MoveFileA(lazer_bootefi, efi_bootefi)) {
        fprintf(stderr, "[-] Unable to replace 'bootx64.efi' - %s\n", GetErrorMessage(GetLastError(), NULL, 0xfff));
        return EXIT_FAILURE;
    }

    /* Install rwlazer64 */
    if (!MoveFileA(lazer_lazerefi, efi_rwlazer_efi)) {
        fprintf(stderr, "[-] Unable to install rwlazer.efi - %s\n", GetErrorMessage(GetLastError(), NULL, 0xfff));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int esp_restore(void) {

    const char* efi_rwlazer_dir = "Q:\\EFI\\rwlazer";
    const char* efi_rwlazer_efi = "Q:\\EFI\\rwlazer\\rwlazer.efi";
    const char* efi_bootefi = "Q:\\EFI\\Boot\\bootx64.efi";
    const char* win_backupefi = "Q:\\EFI\\Boot\\winbootx64.efi";

    if (FileExists(win_backupefi)) {
        /* We dont rely on MoveFileA() FILE_WRITE_THROUGH */
        if (!DeleteFileA(efi_bootefi)) {
            fprintf(stderr, "[-] Unable to delete 'bootx64.efi' in EFI System Partition - %s\n", GetErrorMessage(GetLastError(), NULL, 0xfff));
            return EXIT_FAILURE;
        }

        if (!MoveFileA(win_backupefi, efi_bootefi)) {
            fprintf(stderr, "[-] Unable to restore 'bootx64.efi', leaving you without a bootable system! You may want to do some manual stitching and fixing.. [E:%d - %s]\n", GetLastError(), GetErrorMessage(GetLastError(), NULL, 0xfff));
            return EXIT_FAILURE;
        }

        if (!DeleteFileA(efi_rwlazer_efi)) {
            fprintf(stderr, "[-] Unable to delete 'rwlazer.efi'. This may affect installing RWLAZER in the future. - %s\n", GetErrorMessage(GetLastError(), NULL, 0xfff));
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
    
    fprintf(stderr, "[-] No backup of 'bootx64.efi' was found, aborting..\n");
    return EXIT_FAILURE;
}

void printHelp(void) {
    puts("[---===== RWLAZER INSTALLER HELP =====---]\n[ '--restore'    - Uninstall RWLAZER\n[ '--reinstall'  - Re-install RWLAZER\n[ '--help'       - Show this text");
}

void mountEFI(int *efi_lock) {
    if (*efi_lock == 0) { 
        puts("[+] Mounting EFI SYSTEM..");
        system("mountvol Q: /S");
        *efi_lock = 1;
    }
}

int main(int argc, const char** argv) {
    int EFI_LOCK = 0;
    HANDLE hStdout;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (promptUnsafe() != EXIT_SUCCESS) { return EXIT_SUCCESS; }

    cls(hStdout);

    if (argc > 1) {
        if (strncmp(argv[1], "--restore", 16) == EXIT_SUCCESS) {
            /* restore */
            puts("[+] Uninstalling RWLAZER..");
            mountEFI(&EFI_LOCK);
            if (esp_restore() != EXIT_SUCCESS) { return EXIT_FAILURE; }
            puts("[+] RWLAZER SUCCESSFULLY UNINSTALLED");
            system("pause");
            return EXIT_SUCCESS;
        }
        else if (strncmp(argv[1], "--reinstall", 16) == EXIT_SUCCESS) {
            puts("[+] Re-installing RWLAZER..");
            mountEFI(&EFI_LOCK);
            if (esp_restore() != EXIT_SUCCESS) {
                fprintf(stderr, "[-] Unable to remove RWLAZER installation, aborting..\n");
                system("pause");
                return EXIT_FAILURE;
            }
        }
        else if (strncmp(argv[1], "--help", 16) == EXIT_SUCCESS) {
            printHelp();
            system("pause");
            return EXIT_SUCCESS;
        }
        else {
            fprintf(stderr, "[-] Unknown argument. '%s --help' for more info.\n", argv[0]);
            system("pause");
            return EXIT_FAILURE;
        }
    }

    /* "MOUNT" EFI_SYSTEM */
    //puts("[+] Mounting EFI SYSTEM..");
    //system("mountvol Q: /S");
    mountEFI(&EFI_LOCK);
    
#ifdef _LAZER_MD5_CHECK
    const char* efi_bootefi = "Q:\\EFI\\Boot\\bootx64.efi";
    const char* win_backupefi = "Q:\\EFI\\Boot\\winbootx64.efi";
    const char* efi_lazer64 = "Q:\\EFI\\rwlazer\\rwlazer.efi";
    const char* inst_lazer64 = efi_lazer64 + 3;
    const char* inst_injector = efi_bootefi + 3;
    
    FILE* fp = fopen(efi_lazer64, "rb");
    if (fp == NULL) {
        fprintf(stderr, "[-] Unable to open '%s' for reading - %s\n", efi_lazer64, strerror(errno));
        return EXIT_FAILURE;
    }
    puts("[+] Found 'rwlazer.efi'");
    puts("[+] Comparing hashes..");

    /* This needs to be updated, or pulled from remote, only a prototype */
    const char* precalc_sum = "20b96e754bcf1000c444c4f93b82d0b3";
    char* lazer_sum = MD5SUM_INIT(fp);
    fclose(fp);

    int cmpRes = strncmp(precalc_sum, lazer_sum, MD5_LEN_MAX);
    MD5SUM_FINAL(lazer_sum);
    if (cmpRes != EXIT_SUCCESS) {
        fprintf(stderr, "[-] MD5SUM mismatch: '%s' - '%s'  [Expected: '%s']\n", efi_lazer64, lazer_sum, precalc_sum);
        return EXIT_FAILURE;
    }

    printf("[+] MD5 match: '%s' - %s\n", efi_lazer64, precalc_sum);
#endif

    puts("[+] Installing RWLAZER..");

    /*if (!MoveFileA(efi_bootmgfw, win_backupefi)) {
        fprintf(stderr, "[-] Unable to move '%s' to '%s' - %s\n", efi_bootmgfw, win_backupefi, GetErrorMessage(GetLastError(), NULL, 0xfff));
        return EXIT_FAILURE;
    }

    if (!MoveFileA("bootlazer_x64.efi", efi_bootmgfw)) {
        fprintf(stderr, "[-] Unable to move 'bootlazer_x64.efi' to '%s' - %s\n", efi_bootmgfw, GetErrorMessage(GetLastError(), NULL, 0xfff));
        return EXIT_FAILURE;
    }*/

    if (esp_install() != EXIT_SUCCESS) {
        fprintf(stderr, "[-] Failed to install RWLAZER\n");
        system("pause");
        return EXIT_FAILURE;
    }

    puts("\n[+] RWLAZER SUCCESSFULLY INSTALLED");
    system("pause");

	return EXIT_SUCCESS;
}