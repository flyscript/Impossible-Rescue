@echo off
echo ------------------------------------------------------------------------------
echo:
echo About to remove all empty folders in %CD%
echo:
echo ------------------------------------------------------------------------------
echo:
pause
robocopy . . /s /move /xd .git
pause