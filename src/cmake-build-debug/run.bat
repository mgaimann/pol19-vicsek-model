@echo off
for /l %%x in (100,50,1000) do (
echo %%x
src.exe %%x 100 1 30
@echo.
)
pause