@echo off
echo ----------------------------------------------------
echo Press any key to delete all files with ending:
echo  *.aps *.idb *.ncp *.obj *.pch *.tmp *.sbr *.plg *.dsw *.clw
echo Visual c++/.Net junk 
echo ----------------------------------------------------
pause

del /F /Q /S *.aps *.idb *.ncp *.obj *.pch *.sbr *.tmp *.bsc *.ilk *.ncb *.opt *.suo *.manifest *.dep *.plg *.clw *.sdf
del /F /Q /S *.mine *.smp *.bak *.hm

pause


