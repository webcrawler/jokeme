
::对目录all_src下lua文件进行xxtea（为了兼容先不编译为字节码）加密为luac保存到目录encrypt_lua 
cocos luacompile -s all_src -d encrypt_lua -e -k xxxx -b xxx --disable-compile
::cocos luacompile -s all_src -d encrypt_lua -e -k xxxx -b xxxx --bytecode-64bit