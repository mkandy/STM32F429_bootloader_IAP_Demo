REM Configure option bytes for later swap bank operations
ST-LINK_CLI.exe -c ID=0 SWD -OB nDBOOT=0 nDBANK=0 BOOT_ADD0=0x2000 BOOT_ADD1=0x2040
PAUSE