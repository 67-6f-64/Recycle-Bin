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