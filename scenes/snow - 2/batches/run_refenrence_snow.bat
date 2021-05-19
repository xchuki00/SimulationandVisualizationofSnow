@set NITER=10800
@set PATH_LENGTH=80
@set RESOLUTION=800x350
@set PP3D_RADIUS=-0.001
@set PP3D_ALPHA=1
@set PB2D_RADIUS=-0.001
@set PB2D_ALPHA=1
@set SPM_RADIUS=-0.001
@set SPM_ALPHA=0.75
@set BB1D_RADIUS=-0.001
@set BB1D_ALPHA=1
@set QUERY_BEAM=L
@set PHOTON_BEAM=S
@set IGNORE_SPEC=1
@set COUT=500
@set MODE=-compatible


@set ALG=upbp_pb2d

.\TimeMem-1.0.exe ^
	"..\..\..\..\smallupbp\Build\SmallUPBP\x64\Release\SmallUPBP.exe" -s -1 "..\snow.obj" ^
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
	-th 4 ^
	-pbc 23000 ^
	-seed 2657 ^
	%MODE% ^
	-tgnorespec %IGNORE_SPEC% ^
	-continuous_output %COUT% ^
	-o reference_%ALG%_%NITER%.exr

@set ALG=upbp_bpt

.\TimeMem-1.0.exe ^
	"..\..\..\..\smallupbp\Build\SmallUPBP\x64\Release\SmallUPBP.exe" -s -1 "..\snow.obj" ^
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
	-th 4 ^
	-pbc 23000 ^
	-seed 2657 ^
	%MODE% ^
	-tgnorespec %IGNORE_SPEC% ^
	-continuous_output %COUT% ^
	-o reference_%ALG%_%NITER%.exr

@set ALG=upbp_bb1d

.\TimeMem-1.0.exe ^
	"..\..\..\..\smallupbp\Build\SmallUPBP\x64\Release\SmallUPBP.exe" -s -1 "..\snow.obj" ^
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
	-th 4 ^
	-pbc 23000 ^
	-seed 2657 ^
	%MODE% ^
	-tgnorespec %IGNORE_SPEC% ^
	-continuous_output %COUT% ^
	-o reference_%ALG%_%NITER%.exr

@REM @set ALG=upbp_surf

@REM .\TimeMem-1.0.exe ^
@REM 	"..\..\..\..\smallupbp\Build\SmallUPBP\x64\Release\SmallUPBP.exe" -s -1 "..\snow.obj" ^
@REM 	-a %ALG% ^
@REM 	-l %PATH_LENGTH% ^
@REM 	-t %NITER% ^
@REM 	-r %RESOLUTION% ^
@REM 	-r_initial_pp3d %PP3D_RADIUS% ^
@REM 	-r_alpha_pp3d %PP3D_ALPHA% ^
@REM 	-r_initial_pb2d %PB2D_RADIUS% ^
@REM 	-r_alpha_pb2d %PB2D_ALPHA% ^
@REM 	-r_initial_surf %SPM_RADIUS% ^
@REM 	-r_alpha_surf %SPM_ALPHA% ^
@REM 	-r_initial_bb1d %BB1D_RADIUS% ^
@REM 	-r_alpha_bb1d %BB1D_ALPHA% ^
@REM 	-qbt %QUERY_BEAM% ^
@REM 	-pbt %PHOTON_BEAM% ^
@REM 	-th 4 ^
@REM 	-pbc 23000 ^
@REM 	-seed 2657 ^
@REM 	%MODE% ^
@REM 	-tgnorespec %IGNORE_SPEC% ^
@REM 	-continuous_output %COUT% ^
@REM 	-o reference_%ALG%_%NITER%.exr


@REM @set ALG=upbp_pp3d


@REM .\TimeMem-1.0.exe ^
@REM 	"..\..\..\..\smallupbp\Build\SmallUPBP\x64\Release\SmallUPBP.exe" -s -1 "..\snow.obj" ^
@REM 	-a %ALG% ^
@REM 	-l %PATH_LENGTH% ^
@REM 	-t %NITER% ^
@REM 	-r %RESOLUTION% ^
@REM 	-r_initial_pp3d %PP3D_RADIUS% ^
@REM 	-r_alpha_pp3d %PP3D_ALPHA% ^
@REM 	-r_initial_pb2d %PB2D_RADIUS% ^
@REM 	-r_alpha_pb2d %PB2D_ALPHA% ^
@REM 	-r_initial_surf %SPM_RADIUS% ^
@REM 	-r_alpha_surf %SPM_ALPHA% ^
@REM 	-r_initial_bb1d %BB1D_RADIUS% ^
@REM 	-r_alpha_bb1d %BB1D_ALPHA% ^
@REM 	-qbt %QUERY_BEAM% ^
@REM 	-pbt %PHOTON_BEAM% ^
@REM 	-th 4 ^
@REM 	-pbc 23000 ^
@REM 	-seed 2657 ^
@REM 	%MODE% ^
@REM 	-tgnorespec %IGNORE_SPEC% ^
@REM 	-continuous_output %COUT% ^
@REM 	-o reference_%ALG%_%NITER%.exr

@REM @set ALG=upbp_bpm

@REM  .\TimeMem-1.0.exe ^
@REM 	"..\..\..\..\smallupbp\Build\SmallUPBP\x64\Release\SmallUPBP.exe" -s -1 "..\snow.obj" ^
@REM 	-a %ALG% ^
@REM 	-l %PATH_LENGTH% ^
@REM 	-t %NITER% ^
@REM 	-r %RESOLUTION% ^
@REM 	-r_initial_pp3d %PP3D_RADIUS% ^
@REM 	-r_alpha_pp3d %PP3D_ALPHA% ^
@REM 	-r_initial_pb2d %PB2D_RADIUS% ^
@REM 	-r_alpha_pb2d %PB2D_ALPHA% ^
@REM 	-r_initial_surf %SPM_RADIUS% ^
@REM 	-r_alpha_surf %SPM_ALPHA% ^
@REM 	-r_initial_bb1d %BB1D_RADIUS% ^
@REM 	-r_alpha_bb1d %BB1D_ALPHA% ^
@REM 	-qbt %QUERY_BEAM% ^
@REM 	-pbt %PHOTON_BEAM% ^
@REM 	-pbc 23000 ^
@REM 	-seed 2657 ^
@REM 	%MODE% ^
@REM 	-tgnorespec %IGNORE_SPEC% ^
@REM 	-continuous_output %COUT% ^
@REM 	-o reference_%ALG%_%NITER%.exr

@REM @set ALG=upbp_ptmis

@REM .\TimeMem-1.0.exe ^
@REM 	"..\..\..\..\smallupbp\Build\SmallUPBP\x64\Release\SmallUPBP.exe" -s -1 "..\snow.obj" ^
@REM 	-a %ALG% ^
@REM 	-l %PATH_LENGTH% ^
@REM 	-t %NITER% ^
@REM 	-r %RESOLUTION% ^
@REM 	-r_initial_pp3d %PP3D_RADIUS% ^
@REM 	-r_alpha_pp3d %PP3D_ALPHA% ^
@REM 	-r_initial_pb2d %PB2D_RADIUS% ^
@REM 	-r_alpha_pb2d %PB2D_ALPHA% ^
@REM 	-r_initial_surf %SPM_RADIUS% ^
@REM 	-r_alpha_surf %SPM_ALPHA% ^
@REM 	-r_initial_bb1d %BB1D_RADIUS% ^
@REM 	-r_alpha_bb1d %BB1D_ALPHA% ^
@REM 	-qbt %QUERY_BEAM% ^
@REM 	-pbt %PHOTON_BEAM% ^
@REM 	-pbc 23000 ^
@REM 	-seed 2657 ^
@REM 	%MODE% ^
@REM 	-tgnorespec %IGNORE_SPEC% ^
@REM 	-continuous_output %COUT% ^
@REM 	-o reference_%ALG%_%NITER%.exr


@REM @set ALG=upbp_ptls

@REM .\TimeMem-1.0.exe ^
@REM 	"..\..\..\..\smallupbp\Build\SmallUPBP\x64\Release\SmallUPBP.exe" -s -1 "..\snow.obj" ^
@REM 	-a %ALG% ^
@REM 	-l %PATH_LENGTH% ^
@REM 	-t %NITER% ^
@REM 	-r %RESOLUTION% ^
@REM 	-r_initial_pp3d %PP3D_RADIUS% ^
@REM 	-r_alpha_pp3d %PP3D_ALPHA% ^
@REM 	-r_initial_pb2d %PB2D_RADIUS% ^
@REM 	-r_alpha_pb2d %PB2D_ALPHA% ^
@REM 	-r_initial_surf %SPM_RADIUS% ^
@REM 	-r_alpha_surf %SPM_ALPHA% ^
@REM 	-r_initial_bb1d %BB1D_RADIUS% ^
@REM 	-r_alpha_bb1d %BB1D_ALPHA% ^
@REM 	-qbt %QUERY_BEAM% ^
@REM 	-pbt %PHOTON_BEAM% ^
@REM 	-th 4 ^
@REM 	-pbc 23000 ^
@REM 	-seed 2657 ^
@REM 	%MODE% ^
@REM 	-tgnorespec %IGNORE_SPEC% ^
@REM 	-continuous_output %COUT% ^
@REM 	-o reference_%ALG%_%NITER%.exr




