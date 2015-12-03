@echo off

    if exist "out.obj" del "out.obj"
    if exist "out.exe" del "out.exe"

    masm32\ml.exe /c /coff "out.asm" > nul
    if errorlevel 1 goto errasm

    masm32\polink.exe /SUBSYSTEM:CONSOLE "out.obj" > nul
    if errorlevel 1 goto errlink
    echo **************************************************
    echo                      SUCCESS 
		echo.
		echo                 out.exe ¿ªÊ¼Ö´ÐÐ
	 	echo **************************************************
    out.exe
    goto TheEnd

  :errlink
    echo _
    echo Link error
    goto TheEnd

  :errasm
    echo _
    echo Assembly Error
    goto TheEnd
    
  :TheEnd

