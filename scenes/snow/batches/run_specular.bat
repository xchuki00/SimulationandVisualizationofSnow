@set NITER=1000000
@set ALG=vpts
@set PATH_LENGTH=80
@set RESOLUTION=1600x700
@set COUT=8
@set NAME=specular.exr

"..\..\..\Build\SmallUPBP\x64\Debug\SmallUPBP.exe" -s -1 "..\snow.obj" ^
-a %ALG% ^
-l %PATH_LENGTH% ^
-i %NITER% ^
-r %RESOLUTION% ^
-seed 2657 ^
-continuous_output %COUT% ^
-o %NAME%

./SmallUPBP.exe -s -1 ..\..\..\..\scenes\snow\snow.obj -a upbp_bpm -l 80 -i 10 -r 800x600 -r_initial_pp3d -0.0001 -r_alpha_pp3d 1 -r_initial_pb2d -0.0001 -r_alpha_pb2d 1 -r_initial_surf -0.0001 -r_alpha_surf 0.75 -r_initial_bb1d -0.0001 -r_alpha_bb1d 1 -debugimg_option per_technique -debugimg_output_weights 1 -debugimg_multbyweight output_both -qbt L -pbt S -pbc 1000 -seed 2657 -compatible -ignorespec 1 -continuous_output 100 -o test.exr