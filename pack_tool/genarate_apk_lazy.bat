

::����lua
cd encryptLua
call encrypt_lua.bat

::����ͼƬ
cd ../encryptImage
call encrypt_all_img.bat

::�����ļ�md5�б�
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