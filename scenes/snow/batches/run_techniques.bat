@set NITER=752
@set ALG=bb1d
@set PATH_LENGTH=80
@set RESOLUTION=1600x700
@set PP3D_RADIUS=-0.0001
@set PP3D_ALPHA=1
@set PB2D_RADIUS=-0.0001
@set PB2D_ALPHA=1
@set SPM_RADIUS=-0.0001
@set SPM_ALPHA=0.75
@set BB1D_RADIUS=-0.0001
@set BB1D_ALPHA=1
@set DEBUGIMG_OPTION=per_technique
@set DEBUGIMG_OUTPUTWEIGHTS=1
@set QUERY_BEAM=L
@set PHOTON_BEAM=L
@set IGNORE_SPEC=1
@set COUT=100
@set NAME=t2.exr
@set MODE=-compatible

"..\..\..\SimulationAndVisualizationOfSnow\x64\Release\SimulationAndVisualizationOfSnow.exe" -s -1 "..\snow.obj" ^
-a %ALG% ^
-l %PATH_LENGTH% ^
-i %NITER% ^
-r %RESOLUTION% ^
-r_initial_pp3d %PP3D_RADIUS% ^
-r_alpha_pp3d %PP3D_ALPHA% ^
-r_initial_pb2d %PB2D_RADIUS% ^
-r_alpha_pb2d %PB2D_ALPHA% ^
-r_initial_surf %SPM_RADIUS% ^
-r_alpha_surf %SPM_ALPHA% ^
-r_initial_bb1d %BB1D_RADIUS% ^
-r_alpha_bb1d %BB1D_ALPHA% ^
-debugimg_option %DEBUGIMG_OPTION% ^
-debugimg_output_weights %DEBUGIMG_OUTPUTWEIGHTS% ^
-debugimg_multbyweight output_both ^
-qbt %QUERY_BEAM% ^
-pbt %PHOTON_BEAM% ^
-pbc 23000 ^
-seed 2657 ^
%MODE% ^
-ignorespec %IGNORE_SPEC% ^
-continuous_output %COUT% ^
-o %NAME%