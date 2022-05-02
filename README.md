> Purpose:	Practice UDP/TCP socket networking for CS 3377  
> Author:	Connor Funk  
> Date:		May 01, 2022  
> Version:	2.0  

there are 2 ways to set up the servers
1) you can run 'make run' which will build and run log_s and echo_s in the background this automattically sets the ports of echo_s to 6060 7070 and 8080, this also builds and runs echo_c

2) you can build and run log_s and echo_s manually, by running './log_s &' and './echo_s <port number> [<port number> [<port number>] ]'. this allows you to specify which ports echo_s uses.

to run the client use './echo_c <port number>'. make sure to use one of the ports echo_s is using.
TO EXIT CLIENT send exit to the server

you can run 'make' or 'make build' to build the exes automatically using a 2-step build process

to kill the servers, just run 'make kill', which will kill processses named ./echo_s and .log_s older than 1 sec

you can remove the exes and \*.o's by running 'make clean'


NOTES:
* the log logs empty messages not sent, possibly connect/exit messages
* I don't know why the highlighting on the file exists or how to get rid of them
