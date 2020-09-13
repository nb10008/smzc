@echo off

del /q /s "binary"
del /q /s "temp"


rd /q /s "binary"
rd /q /s "temp"


del /q "*.ncb"
del /q /a:h "*.suo"
del /q "*.user"


