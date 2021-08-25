@echo off 

REM HTTP Server IAP demo project directory
SET "PROJECT_DIR=..\demo\st\nucleo_f767zi"

REM Configure option bytes for later swap bank operations
ST-LINK_CLI.exe -c ID=0 SWD -OB nDBOOT=0 nDBANK=0 BOOT_ADD0=0x2000 BOOT_ADD1=0x2040

REM Load pre-compiled HTTP Server IAP demo banary into flash memory
ST-LINK_CLI.exe -c ID=0 SWD -P %PROJECT_DIR%\http_server_iap_demo\http_server_iap_demo.bin 0x08000000 -Rst
