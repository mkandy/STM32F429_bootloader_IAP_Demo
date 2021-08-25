@echo off 

REM App image builder directory
SET "APP_IMAGE_BUILDER_DIR=F:\DS_STM32F429\IAP\CycloneBOOT_IAP_Demo_Open_1_0_1\utils\ApplicationImageBuilder"

REM HTTP Server IAP demo project directory
SET "PROJECT_DIR=F:\DS_STM32F429\IAP\CycloneBOOT_IAP_Demo_Open_1_0_1\demo\st\nucleo_f429zi"

REM App image builder executable path
SET APP_IMAGE_BUILDER_EXE=%APP_IMAGE_BUILDER_DIR%\app_image_builder.exe

REM Scenario 1 (Simple App + Integrity) image CLI options
SET SCN_1_INPUT=%PROJECT_DIR%\http_server_iap_demo\http_server_iap_demo.bin
SET SCN_1_OUTPUT=%PROJECT_DIR%\http_server_iap_demo\nucleo_f429zi_http_server_iap_image_scenario_1.img
SET SCN_1_INTEGRITY=crc32

REM Scenario 2 (Cipher App + Authentication) image CLI options
SET SCN_2_INPUT=%PROJECT_DIR%\http_server_iap_demo\http_server_iap_demo.bin
SET SCN_2_OUTPUT=%PROJECT_DIR%\http_server_iap_demo\nucleo_f429zi_http_server_iap_image_scenario_2.img
SET SCN_2_CIPHER=aes-cbc
SET SCN_2_CIPHER_KEY=aa3ff7d43cc015682c7dfd00de9379e7
SET SCN_2_AUTH=hmac-md5
SET SCN_2_AUTH_KEY=107b8d4bf62d1c9e26968e8ff51a74df

REM Scenario 3 (Cipher App + Signature) image CLI options
SET SCN_3_INPUT=%PROJECT_DIR%\http_server_iap_demo\http_server_iap_demo.bin
SET SCN_3_OUTPUT=%PROJECT_DIR%\http_server_iap_demo\nucleo_f429zi_http_server_iap_image_scenario_3.img
SET SCN_3_CIPHER=aes-cbc
SET SCN_3_CIPHER_KEY=aa3ff7d43cc015682c7dfd00de9379e7
SET SCN_3_SIGN=rsa-sha256
SET SCN_3_SIGN_KEY=%PROJECT_DIR%\http_server_iap_demo\resources\keys\rsa_private_key.pem

echo =========================
echo =   APP IMAGE BUILDER   =
echo =========================
echo.

REM Generate Scenario 1 (Simple App + Integrity) image
echo -^> Generating Scenario 1 (Simple App + Integrity) image...
%APP_IMAGE_BUILDER_EXE% -input %SCN_1_INPUT% -output %SCN_1_OUTPUT% -i %SCN_1_INTEGRITY% >nul
echo.

REM Generate Scenario 2 (Cipher App + Authentication) image
echo -^> Generating Scenario 2 (Cipher App + Authentication) image...
%APP_IMAGE_BUILDER_EXE% -input %SCN_2_INPUT% -output %SCN_2_OUTPUT%^
    -e %SCN_2_CIPHER% -ek %SCN_2_CIPHER_KEY% -a %SCN_2_AUTH% -ak %SCN_2_AUTH_KEY% >nul
echo.

REM Generate Scenario 3 (Cipher App + Signature) image
echo -^> Generating Scenario 3 (Cipher App + Signature) image...
%APP_IMAGE_BUILDER_EXE% -input %SCN_3_INPUT% -output %SCN_3_OUTPUT%^
    -e %SCN_3_CIPHER% -ek %SCN_3_CIPHER_KEY% -s %SCN_3_SIGN% -sk %SCN_3_SIGN_KEY% >nul
echo.

echo Done.

PAUSE
