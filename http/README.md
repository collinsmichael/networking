# http
http is a bare bones http web server.

the root sub-directory acts as the web root folder
http will only search for html files within root/page.html

http only understands GET requests (treats everything as a GET)
serves back a 404 response if the file is not found
serves back a 200 response if the file exists
