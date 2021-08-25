@echo off 

REM HTTP Server IAP demo project directory
SET "PROJECT_DIR=..\demo\st\nucleo_f429zi"

REM Configure option bytes for later swap bank operations
ST-LINK_CLI.exe -c ID=0 SWD -OB BFB2=0

REM Load pre-compiled HTTP Server IAP demo banary into flash memory
ST-LINK_CLI.exe -c ID=0 SWD -P %PROJECT_DIR%\http_server_iap_demo\http_server_iap_demo.bin 0x08000000 -Rst

PAUSE
