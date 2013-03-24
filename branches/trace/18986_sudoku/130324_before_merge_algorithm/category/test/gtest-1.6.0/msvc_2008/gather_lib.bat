@echo off
ECHO Clean lib
rd /s /q lib
md lib
rem
ECHO Copy /MD to lib
copy gtest-md\Debug\gtest_main-mdd.lib	lib\gtest_main-mdd.lib
copy gtest-md\Release\gtest_main-md.lib lib\gtest_main-md.lib
copy gtest-md\Debug\gtestd.lib		lib\gtest_mdd.lib
copy gtest-md\Release\gtest.lib		lib\gtest_md.lib
rem
ECHO Copy /MT to lib
copy gtest\Debug\gtest_maind.lib	lib\gtest_main-mtd.lib
copy gtest\Release\gtest_main.lib	lib\gtest_main-mt.lib
copy gtest\Debug\gtestd.lib		lib\gtest_mtd.lib
copy gtest\Release\gtest.lib		lib\gtest_mt.lib
rem
ECHO Copy "gtest-1.6.0\include\gtest"* to "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include\gtest"
ECHO Copy "lib\"* to "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\lib"
pause
