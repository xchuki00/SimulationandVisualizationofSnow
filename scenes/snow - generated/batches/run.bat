@set NITER=100
@set ALG=upbp_bb1d
@set PATH_LENGTH=160
@set RESOLUTION=1600x700
@set BB1D_RADIUS=1.0
@set BB1D_ALPHA=0.01
@set DEBUGIMG_OPTION=per_technique
@set DEBUGIMG_OUTPUTWEIGHTS=1
@set QUERY_BEAM=L
@set PHOTON_BEAM=L
@set IGNORE_SPEC=1
@set COUT=0
@set NAME=ptpb-14.5.exr
@set MODE=-compatible

"..\..\..\SimulationAndVisualizationOfSnow\x64\Release\SimulationAndVisualizationOfSnow.exe" -s -1 "..\snow.obj" ^
-a %ALG% ^
-l %PATH_LENGTH% ^
-i %NITER% ^
-r %RESOLUTION% ^
-r_initial_bb1d %BB1D_RADIUS% ^
-r_alpha_bb1d %BB1D_ALPHA% ^
-qbt %QUERY_BEAM% ^
-pbt %PHOTON_BEAM% ^
-th 4 ^
-pbc 24000 ^
-seed 1234 ^
%MODE% ^
-ignorespec %IGNORE_SPEC% ^
-continuous_output %COUT% ^
-o %NAME%