:: Lynbrook scripts for building the project.
::   Add the build number and reformat the c++ code.
::   Note: we start in the folder "PPC603gnu"
::   B.Axlerod '13, K.Viswanathan '11, D.Giandomenico

cd ../Config
perl build.pl

cd ../BuildScripts
call BuildNumber_Increment.cmd ../BuildNumber.h

cd ..
astyle "*.cpp" "*.h"

::Don't mess with changes to astyle  right now. Wait for Brian. -dg
::astyle --options=astylerc "*.cpp" "*.h"

cd PPC603gnu
%makeprefix% make --no-print-directory %*