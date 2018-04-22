set T=%date%_%time%
set T=%T:/=%
set T=%T::=%
set T=%T: =%
set T=%T:~,-3%
mkdir "../Archive/"%T%
copy "*.h" "../Archive/"%T%
copy "*.cpp" "../Archive/"%T%
copy "*.rc" "../Archive/"%T%
copy "*.manifest" "../Archive/"%T%
