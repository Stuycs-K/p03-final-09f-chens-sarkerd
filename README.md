[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)
# Battleship
 
### Dabir Fan Club

Sean Chen

Debojit Sarker
       
### Project Description:

Server takes clients 2 at a time. The server will block for response from client 1, then response from client 2. It will receive the responses and then send that response to the opposite server. Follows the rules of battleship, but much smaller grid to showcase easily.

client will display bomb/ship screens:

example ship screen:  
  1 2 3  
A 0 0 0    
B X 0 0   
C S S 0  


S = ship
0 = water
X = hit

example bomb screen:  
X 0 O
O 0 0
H X 0

X = fully dead ship 
H = hit ship (becomes X when that specific ship is sunk)
O = miss
0 = Water



### Instructions:

[WILL CHANGE WITH DEVELOPMENT]

first client(P1) hosts the server. 

second client(P2) connects to server. 

client prompts for the two ship locations, one at a time. players type in where the ships are. 

client prompts for bomb location, one at a time. players type in where they want to drop the bomb, and the client will show different symbols depending on if it hits or misses. 

### Resources/ References:


