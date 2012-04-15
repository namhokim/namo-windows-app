1. install nod.js
 http://www.nodejs.org/#download
  x86: http://nodejs.org/dist/v0.6.15/
  x64: http://nodejs.org/dist/v0.6.15/x64/
2. install dependenced jade module
 (if not install, you meet "Error: Cannot find module 'jade'")
 > npm install jade
2. C plugin build - whoConnect.exe , sqlite3(option, if using ip:name map)
 Copy 'whoConnect.exe' file into directory of same as 'requestHandlers.js'.
3. Add map table to mapping name (option)
 Make mapping data with CSV format with utf8 encoding. (eg. map.csv)
  [map.csv]
  192.168.1.1,Arthor
  192.168.1.2,James
  ...
 > whoConnect.exe
  then created file as ip_map.db
 > sqlite3 ip_map.db
 > .import map.csv map
4. Run Node.js
 > node index.js
5. Make Service