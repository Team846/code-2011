cd ..\Config
perl build.pl
cd ..\PPC603gnu
%makeprefix% make --no-print-directory %CmdCmdLine:~22%