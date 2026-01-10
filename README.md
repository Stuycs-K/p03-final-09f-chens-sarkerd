[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)
# Battleship
 
### Dabir Fan Club

Sean Chen

Debojit Sarker
       
### Project Description:

Server takes clients 2 at a time. The server will block for response from client 1, then response from client 2. It will receive the responses and then send that response to the opposite server. Follows the rules of battleship, but much smaller grid to showcase easily.

client will display bomb/ship screens:

example ship screen:   
. . .    
X . .   
S S .  


S = ship
. = water
X = hit

example bomb screen:    
X . O  
O . .  
. X .  

X = dead ship 
O = miss
. = Water

[EXTRA FEATURES] (check mark means done, will only do if we have time)
- bomb special (hit 2 squares with 1 fire)
- hitting consecutive times (debatable to keep/remove, since the game is kept on a 3x3 it might be unfair and end game too fast)
- handle multiple games (ex. 2 clients connect, game 1, third client connects and waits for fourth client to start game 2 and so on)



### Instructions:

[WILL CHANGE WITH DEVELOPMENT]

first client(P1) hosts the server. 

second client(P2) connects to server. 

client prompts for the two ship locations, one at a time. players type in where the ships are. 

client prompts for bomb location, one at a time. players type in where they want to drop the bomb, and the client will show different symbols depending on if it hits or misses. 

### Resources/ References:


