setlocal
@pushd

@echo argument 1 is %1%
@echo argument 2 is %2%


:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: constants
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: sub folder containing texture packer files and assets
@set SUBFOLDER_TEXTUREPACKER=TexturePacker

:: this is the path to the texture packer exe
@set TEXTUREPACKER_EXE="C:\Program Files\CodeAndWeb\TexturePacker\bin\TexturePacker.exe"

:: sub folder containing fonts
@set SUBFOLDER_FONTS=fonts

:: sub folder containing loose resource files that don't get processed
@set SUBFOLDER_LOOSE=Loose

:: sub folder containing physics editor files and assets
@set SUBFOLDER_PHYSICSEDITOR=PhysicsEditor

:: sub folder containing ogmo editor files
@set SUBFOLDER_OGMOEDITOR=OgmoEditor

:: sub folder containing music files
@set SUBFOLDER_MUSIC=music

:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: parameters - we resolve these to absolute paths
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

:: SOURCEFOLDER_IN is 1st param 
@set SOURCEFOLDER_IN=%1%

::resolve to abs path
@pushd
@cd %SOURCEFOLDER_IN%
@set SOURCEFOLDER_ABS=%CD%
@popd

@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@echo @ Data Build Starting
@echo @
@echo @ Source Asset Folder: %SOURCEFOLDER_ABS%

:: output folderOUTPUTFOLDER_IN is 2nd param of batch file
@set OUTPUTFOLDER_IN=%2%

::resolve to abs path
@pushd
@cd %OUTPUTFOLDER_IN%
@set OUTPUTFOLDER_ABS=%CD%
@popd

@echo @ Output Asset Folder: %OUTPUTFOLDER_ABS%
@echo @
@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: start build
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: mirror the folder structure so file writes don't fail, but don't copy files

:: need to exclude some folders from the build using xcopy's exclude flag 
:: these must be specified in a file!
@set TEMP_XCOPY_EXCLUDE_FILE=%SOURCEFOLDER_ABS%\__XCOPY_EXCLUDE__
@echo Frames > %TEMP_XCOPY_EXCLUDE_FILE%

:: /t = copy directory structure but not files
:: /e = include empty directories
:: /r = overwrite readonly files
:: /y = suppress overwrite confimration
:: /EXCLUDE:%TEMP_XCOPY_EXCLUDE_FILE% - see if you can guess!
@xcopy /t /e /r /y /EXCLUDE:%TEMP_XCOPY_EXCLUDE_FILE% %SOURCEFOLDER_ABS% %OUTPUTFOLDER_ABS%


:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: build the .tps files
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: set the folder that the .tps files exist in
@set SOURCEFOLDER_TPSFILES=%SOURCEFOLDER_ABS%\%SUBFOLDER_TEXTUREPACKER%

@echo ----------------------------------------------------------------------------------------------------
@echo - Building Texture Packer Files
@echo -
@echo - Source folder: %SOURCEFOLDER_TPSFILES%
@echo - Output folders determined by settings in .tps files
@echo ----------------------------------------------------------------------------------------------------

:: filename for the temp file containing all the .tps files
@set _TPSLIST_FILENAME_=%SOURCEFOLDER_TPSFILES%\__tpsfiles__

:: change folder into the one with all the .tps files in it
@pushd
@cd %SOURCEFOLDER_TPSFILES%

:: find all the .tps files and pop them into a temporary file
@dir /B /S *.tps > %_TPSLIST_FILENAME_%

:: iterate through the temp file line by line running texture packer on each
for /F "tokens=*" %%A in (%_TPSLIST_FILENAME_%) do %TEXTUREPACKER_EXE% --quiet --smart-update %%A
@popd


:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: copy fonts
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@set SOURCEFOLDER_FONTS=%SOURCEFOLDER_ABS%\%SUBFOLDER_FONTS%
@set OUTPUTFOLDER_FONTS=%OUTPUTFOLDER_ABS%\%SUBFOLDER_FONTS%

@echo ----------------------------------------------------------------------------------------------------
@echo - Copying Fonts
@echo -
@echo -	Source folder: %SOURCEFOLDER_FONTS%
@echo -	Output folder: %OUTPUTFOLDER_FONTS%
@echo ----------------------------------------------------------------------------------------------------
:: /mir - mirror the folder structure
:: /xo	- excludes older files (i.e. only copy if newer)
:: other switches reduce output verbosity
@robocopy /mir /xo /njh /njs /ns /nc /ndl /np %SOURCEFOLDER_FONTS% %OUTPUTFOLDER_FONTS%


:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: copy loose resources
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: sub folder containing loose resource files that don't get processed
@set SOURCEFOLDER_LOOSE=%SOURCEFOLDER_ABS%\%SUBFOLDER_LOOSE%
@set OUTPUTFOLDER_LOOSE=%OUTPUTFOLDER_ABS%\%SUBFOLDER_LOOSE%

@echo ----------------------------------------------------------------------------------------------------
@echo - Copying Loose Assets
@echo -
@echo -	Source folder: %SOURCEFOLDER_LOOSE%
@echo -	Output folder: %OUTPUTFOLDER_LOOSE%
@echo ----------------------------------------------------------------------------------------------------
:: /mir - mirror the folder structure
:: /xo	- excludes older files (i.e. only copy if newer)
:: other switches reduce output verbosity
@robocopy /mir /xo /njh /njs /ns /nc /ndl /np %SOURCEFOLDER_LOOSE% %OUTPUTFOLDER_LOOSE%


:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: copy physics editor resources
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: sub folder containing loose resource files that don't get processed
@set SOURCEFOLDER_PHYSED=%SOURCEFOLDER_ABS%\%SUBFOLDER_PHYSICSEDITOR%
@set OUTPUTFOLDER_PHYSED=%OUTPUTFOLDER_ABS%\%SUBFOLDER_PHYSICSEDITOR%

@echo ----------------------------------------------------------------------------------------------------
@echo - Copying Physics Editor Assets
@echo -
@echo -	Source folder: %SOURCEFOLDER_PHYSED%
@echo -	Output folder: %OUTPUTFOLDER_PHYSED%
@echo ----------------------------------------------------------------------------------------------------
:: /mir - mirror the folder structure
:: /xo	- excludes older files (i.e. only copy if newer)
:: /xf	- exclude files matching following wildcard
:: other switches reduce output verbosity
@robocopy /mir /xo /njh /njs /ns /nc /ndl /np %SOURCEFOLDER_PHYSED% %OUTPUTFOLDER_PHYSED% /xf *.pes


:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: copy ogmo editor resources
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: sub folder containing loose resource files that don't get processed
@set SOURCEFOLDER_OGMO=%SOURCEFOLDER_ABS%\%SUBFOLDER_OGMOEDITOR%
@set OUTPUTFOLDER_OGMO=%OUTPUTFOLDER_ABS%\%SUBFOLDER_OGMOEDITOR%

@echo ----------------------------------------------------------------------------------------------------
@echo - Copying OGMO Editor level files
@echo -
@echo -	Source folder: %SOURCEFOLDER_OGMO%
@echo -	Output folder: %OUTPUTFOLDER_OGMO%
@echo ----------------------------------------------------------------------------------------------------
:: /mir - mirror the folder structure
:: /xo	- excludes older files (i.e. only copy if newer)
:: /xf	- exclude files matching following wildcard
:: other switches reduce output verbosity
@robocopy /mir /xo /njh /njs /ns /nc /ndl /np %SOURCEFOLDER_OGMO% %OUTPUTFOLDER_OGMO% /xf *.oep

:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: copy music
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@set SOURCEFOLDER_MUSIC=%SOURCEFOLDER_ABS%\%SUBFOLDER_MUSIC%
@set OUTPUTFOLDER_MUSIC=%OUTPUTFOLDER_ABS%\%SUBFOLDER_MUSIC%

@echo ----------------------------------------------------------------------------------------------------
@echo - Copying Music
@echo -
@echo -	Source folder: %SOURCEFOLDER_MUSIC%
@echo -	Output folder: %OUTPUTFOLDER_MUSIC%
@echo ----------------------------------------------------------------------------------------------------
:: /mir - mirror the folder structure
:: /xo	- excludes older files (i.e. only copy if newer)
:: other switches reduce output verbosity
@robocopy /mir /xo /njh /njs /ns /nc /ndl /np %SOURCEFOLDER_MUSIC% %OUTPUTFOLDER_MUSIC%


:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: clear temporary files
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@del %TEMP_XCOPY_EXCLUDE_FILE%
@del %_TPSLIST_FILENAME_%

:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
:: write builddatatimestamp.txt
:: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@echo Only this file's timestamp is used > %OUTPUTFOLDER_ABS%\builddatatimestamp.txt

@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@echo Data Build Finished
@echo @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@popd
endlocal
