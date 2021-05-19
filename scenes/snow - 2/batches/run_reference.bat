@set NITER=7200
@set ALG=upbp_all
@set PATH_LENGTH=12
@set RESOLUTION=800x350
@set PP3D_RADIUS=-0.0001
@set PP3D_ALPHA=1
@set PB2D_RADIUS=-0.0001
@set PB2D_ALPHA=1
@set SPM_RADIUS=-0.0015
@set SPM_ALPHA=0.75
@set BB1D_RADIUS=-0.0001
@set BB1D_ALPHA=1
@set QUERY_BEAM=L
@set PHOTON_BEAM=S
@set COUT=500
@set NAME=reference.exr

"..\..\..\SimulationAndVisualizationOfSnow\Build\x64\Release\SmallUPBP.exe" -s -1 "..\snow.obj" ^
-a %ALG% ^
-l %PATH_LENGTH% ^
-t %NITER% ^
-r %RESOLUTION% ^
-r_initial_pp3d %PP3D_RADIUS% ^
-r_alpha_pp3d %PP3D_ALPHA% ^
-r_initial_pb2d %PB2D_RADIUS% ^
-r_alpha_pb2d %PB2D_ALPHA% ^
-r_initial_surf %SPM_RADIUS% ^
-r_alpha_surf %SPM_ALPHA% ^
-r_initial_bb1d %BB1D_RADIUS% ^
-r_alpha_bb1d %BB1D_ALPHA% ^
-qbt %QUERY_BEAM% ^
-pbt %PHOTON_BEAM% ^
-pbc 4000 ^
-maxMemPerThread 2000 ^
-th 4 ^
-continuous_output %COUT% ^
-o %NAME%