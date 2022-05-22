
7z a -tzip game.zip Build/WEB -x!Build/WEB/Data/* 
echo && butler push game.zip %1/%2:%3

