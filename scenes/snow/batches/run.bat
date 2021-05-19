@set NITER=7200
@set PATH_LENGTH=12
@set RESOLUTION=800x350
@set BB1D_RADIUS=1.0
@set BB1D_ALPHA=0.01
@set QUERY_BEAM=L
@set PHOTON_BEAM=L
@set NAME=ptpb-18.5.exr

"..\..\..\SimulationAndVisualizationOfSnow\SimulationAndVisualizationOfSnow.exe" -s -1 "..\snow.obj" ^
-l %PATH_LENGTH% ^
-t %NITER% ^
-r %RESOLUTION% ^
-r_initial_bb1d %BB1D_RADIUS% ^
-r_alpha_bb1d %BB1D_ALPHA% ^
-qbt %QUERY_BEAM% ^
-pbt %PHOTON_BEAM% ^
-th 4 ^
-pbc 24000 ^
-seed 1234 ^
-o PTPB_%NITER%.exr