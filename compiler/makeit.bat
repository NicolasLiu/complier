@echo off

    if exist "out.obj" del "out.obj"
    if exist "out.exe" del "out.exe"

    masm32\ml.exe /c /coff "out.asm" 
    if errorlevel 1 goto errasm

    masm32\polink.exe /SUBSYSTEM:CONSOLE "out.obj" 
    if errorlevel 1 goto errlink
    echo **************************************************
    echo                      SUCCESS 
		echo.
		echo                 out.exe ¿ªÊ¼Ö´ÐÐ
	 	echo **************************************************
    out.exe
    goto TheEnd

  :errlink
    echo Link error
    goto TheEnd

  :errasm
    echo Assembly Error
    goto TheEnd
    
  :TheEnd

