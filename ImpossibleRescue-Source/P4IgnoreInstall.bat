@ sets p4 environment variable to point to p4 ignore file (like a git ignore)
set _P4IGNOPRE_DIR_=%CD%
@echo current folder %_P4IGNOPRE_DIR_%

pushd C:\Program Files\Perforce

@echo perforce exe folder: %CD%

p4 set P4IGNORE=%_P4IGNOPRE_DIR_%\.P4IGNORE

p4 set
pause
