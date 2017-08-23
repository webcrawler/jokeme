

::加密lua
cd encryptLua
call encrypt_lua.bat

::加密图片
cd ../encryptImage
call encrypt_all_img.bat

::生成文件md5列表
cd ../generate_file_md5
call genarate_file_md5.bat
call copy_res_to_dir.bat

set /p bRelease=<"host_model"
if "%bRelease%" == "false" ( 
@echo "model: debug"
) else (
@echo "model: release"
)


cd ../genarate_apk
call genarate_apk_release.bat
pause