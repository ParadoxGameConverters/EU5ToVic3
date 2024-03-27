echo on
rem Copy converter data files
copy "Data_Files\log.txt" "..\Debug\EU5ToVic3\"
copy "Data_Files\configuration-example.txt" "..\Debug\EU5ToVic3\"
copy "Resources\rakaly.dll" "..\Debug\EU5ToVic3\"

mkdir "..\Debug\Configuration"
copy "Data_Files\fronter-configuration.txt" "..\Debug\Configuration\"
copy "Data_Files\fronter-options.txt" "..\Debug\Configuration\"
copy "Data_Files\*.yml" "..\Debug\Configuration\"

mkdir "..\Debug\Docs"
copy "Data_Files\ReadMe.txt" "..\Debug\Docs\"
copy "Data_Files\FAQ.txt" "..\Debug\Docs\"
copy "Data_Files\license.txt" "..\Debug\Docs\"
copy "Data_Files\after_converting.txt" "..\Debug\Docs\"

rem Create Configurables
del "..\Debug\EU5ToVic3\configurables" /Q
rmdir "..\Debug\EU5ToVic3\configurables" /S /Q
xcopy "Data_Files\configurables" "..\Debug\EU5ToVic3\configurables" /Y /E /I

rem Create Blank Mod
del "..\Debug\EU5ToVic3\blankMod" /Q
rmdir "..\Debug\EU5ToVic3\blankMod" /S /Q
xcopy "Data_Files\blankMod" "..\Debug\EU5ToVic3\blankMod" /Y /E /I
