#Command Line Chess Game
in the game black and white squares are represented using different ASCII characters,
e.g. “.” for black squares and “-” for white squares.
White pieces are represented by “p”,“r”,“n”,“b”,“q”,“k” for pawns, rooks, knights,
bishops, queen and king respectively. Black pieces are represented by the same letters but capitalized.
##Movement: 
each square is indexed by a character and a number (e.g. D1,C3,...etc.). 
The user specifies his next move by entering the index of the piece he wishes to move, 
followed by the index of the square he wishes to move it to.
(e.g. A3B4 will move the piece at A3 to the square B4). 
######exception in the case of promotion
The user must specify an additional character indicating the desired piece. 
(e.g. H7H8B will promote the pawn to a bishop)

##Undo and Redo: 
The game could be undone till the first move.
##Save and Load:
- At any time, the game could be saved to a file
- A saved game could be loaded and continued
