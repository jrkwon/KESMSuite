@echo off
rem -------------------------------------------------
rem KESM SUITE 
rem 
rem Change HOMEDRIVE for your system
rem -------------------------------------------------

set KESMSUITE_HOMEDRIVE=P:
set KESMSUITE_HOMEPATH=%KESMSUITE_HOMEDRIVE%\KESMSuite
SET PATH=%PATH%;%KESMSUITE_HOMEPATH\bin%;%KESMSUITE_HOMEDRIVE%\Tools\QtSDK\Desktop\Qt\4.7.4\msvc2008\bin;%KESMSUITE_HOMEDRIVE%\Tools\ITK\bin;

@echo Welcome KESMSuite.
@echo KESMSuite Path is %KESMSUITE_HOMEPATH%.
@echo on
cmd.exe /k