set T=%date%_%time%
set T=%T:/=%
set T=%T::=%
set T=%T: =%
set T=%T:~,-3%
set FilePath="../Lib/"%T%
mkdir %FilePath%
cd ..
cd AirWindow
xcopy "*.h" %FilePath%
cd ..
cd Release
xcopy "*.lib" %FilePath%
cd ..
cd Lib
cd %T%
rename "MakeLib.lib" "AirWindow.lib"

