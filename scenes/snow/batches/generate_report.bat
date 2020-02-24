@set NTIME=3500
@set MODE=-compatible
@set PREVBB1D=-previous_bb1d
@set ALG=upbp_bb1d+pb2d+pp3d+bpt
@set NAME=compatible_all_prevbb1d.exr
call run_temp.bat

@set PREVBB1D=
@set MODE=-compatible
@set ALG=pb2d L
@set NAME=pb2d.exr
call run_temp.bat

@set ALG=vptd
@set NAME=vptd.exr
call run_temp.bat

@set ALG=bb1d
@set NAME=bb1d.exr
call run_temp.bat

@set ALG=upbp_pp3d
@set NAME=pp3d.exr
call run_temp.bat

@set ALG=upbp_bpt
@set NAME=upbp_bpt.exr
call run_temp.bat

