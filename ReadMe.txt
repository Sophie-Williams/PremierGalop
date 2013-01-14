 ###################################
 #### Premier Galop application ####
 ###################################



      #Aa
      #=#
      #=#
      #=#
      #=#
      #=#
d######=#######
D======$======B
#######=######b
      #=#
      #=#
      #=#
      #=#
      #=#
      cC#
      
Normal topology.
a is the start case of the player 0
A is the base of the ladder case of the player 0
b is the start case of the player 1
B is the base of the ladder case of the player 1
c is the start case of the player 2
C is the base of the ladder case of the player 2
d is the start case of the player 3
D is the base of the ladder case of the player 3
= is the ladder

When reading the file all data are stored into a table.
A first analysis of the table is needed in order to determine the number of players.
The table is parsed recursively from the 'a' position, and finish to the a position.
When a end case is encountered, the ladder is parsed.

