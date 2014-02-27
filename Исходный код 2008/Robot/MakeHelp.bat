@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by ROBOT.HPJ. >"hlp\robot.hm"
echo. >>"hlp\robot.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\robot.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\robot.hm"
echo. >>"hlp\robot.hm"
echo // Prompts (IDP_*) >>"hlp\robot.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\robot.hm"
echo. >>"hlp\robot.hm"
echo // Resources (IDR_*) >>"hlp\robot.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\robot.hm"
echo. >>"hlp\robot.hm"
echo // Dialogs (IDD_*) >>"hlp\robot.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\robot.hm"
echo. >>"hlp\robot.hm"
echo // Frame Controls (IDW_*) >>"hlp\robot.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\robot.hm"
REM -- Make help for Project ROBOT


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\robot.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\robot.hlp" goto :Error
if not exist "hlp\robot.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\robot.hlp" Debug
if exist Debug\nul copy "hlp\robot.cnt" Debug
if exist Release\nul copy "hlp\robot.hlp" Release
if exist Release\nul copy "hlp\robot.cnt" Release
echo.
goto :done

:Error
echo hlp\robot.hpj(1) : error: Problem encountered creating help file

:done
echo.
