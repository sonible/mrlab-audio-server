@echo off
:: Version von Git abfragen (Tag-Commits-Hash)
for /f "tokens=*" %%i in ('git describe --tags --long --always') do set GIT_VERSION=%%i

:: Version in eine Datei schreiben (z.B. version.txt)
echo %GIT_VERSION% > resources\WebGUI\version.txt

:: Optionale Ausgabe in der Konsole
echo Version auf %GIT_VERSION% aktualisiert.