# Final Project Proposal

## Group Members:

Sean Chen

Debojit Sarker
       
# Intentions:

Server that allows clients to connect two at a time to play battleship against each other, can handle multiple instances. 


# Intended usage:

ASCII board for displaying ships/bombs 

Prompts the players for ship position, where to throw bombs. 
  
# Technical Details: 

Sockets : server and clients, manage messages, manage matchmaking - Sean

Allocating Memory : managing ships and grid - Sean 

Signals : catch keyboard inputs - Debojit

Processes: allows our server to handle multiple matches - Debojit

note: these are not strict responsibilities, obviously we are helping each other work on things

[Possible Extras if Time]

Files + Finding Info About Files : Leaderboard with match wins and usernames
bomb special (hit 2 squares with 1 fire)
hitting consecutive times (debatable to keep/remove, since the game is kept on a 3x3 it might be unfair and end game too fast)
handle multiple games (ex. 2 clients connect, game 1, third client connects and waits for fourth client to start game 2 and so on)
handle early disconnects (win by default) 

    
# Intended pacing:

1/9: Working Server System/Forks ✔️

1/12: Server has to handle at least one battleship match ✔️

1/16: Polished game, file stuff if extra time
