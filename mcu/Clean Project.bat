@echo off
echo Cleaning project build artifacts...
del /s /q *.o 2>nul
del /s /q *.out 2>nul
del /s /q *.elf 2>nul
del /s /q *.bin 2>nul
del /s /q *.hex 2>nul
del /s /q *.lst 2>nul
del /s /q *.map 2>nul
del /s /q *.d 2>nul
if exist build rmdir /s /q build
if exist output rmdir /s /q output
if exist Project\MDK\Objects rmdir /s /q Project\MDK\Objects
if exist Project\MDK\Listings rmdir /s /q Project\MDK\Listings
if exist Project\IAR\APM32F403 rmdir /s /q Project\IAR\APM32F403
if exist Project\Eclipse\Debug rmdir /s /q Project\Eclipse\Debug
echo Done.
pause
