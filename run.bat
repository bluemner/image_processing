REM FOR %%G IN (3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20) DO (.\bin\ReadWritePGMx.exe ./data/street.pgm ./bin/windows_run/windows_street_%%G.pgm %%G)
REM FOR %%G IN (3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20) DO  (.\bin\ReadWritePGMx.exe ./data/woman.pgm ./bin/windows_run/windows_woman_%%G.pgm %%G)
REM FOR %%G IN (3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20) DO  (.\bin\ReadWritePGMx.exe ./data/ship.pgm ./bin/windows_run/windows_ship_%%G.pgm %%G)
bin/hw1.exe data/mri.pgm image_1-5.pgm 0.5
bin/hw1.exe data/mri.pgm image_2-5.pgm 2.5
bin/hw1.exe data/mri.pgm image_5-0.pgm 5
