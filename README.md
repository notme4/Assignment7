> Purpose:	Practice UDP/TCP socket networking for CS 3377 (Programming in Unix Environment)
> Author:	Connor Funk
> Date:		Apr. 19, 2022

there are 2 ways to set up the servers
1) you can run 'make run' which will build and run log_s and echo_s in the background this automatically sets the ports of echo_s to 6060 7070 and 8080

2) you can build and run log_s and echo_s manually, by running './log_s &' and './echo_s <port number> [<port number> [<port number>] ]'. this allows you to specify which ports echo_s uses.

to run the client use './echo_c <port number>'. make sure to use one of the ports echo_s is using.
TO EXIT CLIENT send exit to the server

you can run 'make' or 'make all' to build the exes automatically using a 2-step build process

to kill the servers, just run 'make kill', which will kill processses named ./echo_s and .log_s older than 1 sec

NOTES:
* the log logs empty messages not sent, possibly connect/exit messages
* Assignment 7 was to put assignment 6 on github, with some minor changes logged with git
