REM server x64
midl /x64 /Os /app_config cachefilesystemproto.idl
REM client win32
midl /win32 /Os /app_config /out testclient cachefilesystemproto.idl