set T=%date%_%time%
set T=%T:/=%
set T=%T::=%
set T=%T: =%
set T=%T:~,-3%
set FilePath="../Archive/"%T%
mkdir %FilePath%
xcopy "*.h" %FilePath%
xcopy "*.cpp" %FilePath%
xcopy "*.rc" %FilePath%
xcopy "*.manifest" %FilePath%
set T=%date%_%time%
set T=%T:/=%
set T=%T::=%
set T=%T: =%
set T=%T:~,-3%
set FilePath="../Lib/"%T%
mkdir %FilePath%
cd ..
cd AirMemory
xcopy "*.h" %FilePath%
cd ..
cd Release
xcopy "*.lib" %FilePath%
cd ..
cd Lib
cd %T%