
chcp 65001 
echo off
color 0B

set cur_dir=%~dp0


python %cur_dir%MultiChannelBuildTool.py

pause