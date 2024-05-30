import sets, sequtils

let content = readFile("2015/3/puzzle.txt")

var santaX,roboX = 0
var santaY, roboY = 0

var visitedHouses = initHashSet[(int, int)]()

visitedHouses.incl((santaX, santaY))
visitedHouses.incl((roboX, roboY))

for i, move in content.pairs:
    if i mod 2 == 0:
        if move == '^':
            santaY += 1
        elif move == '<':
            santaX -= 1
        elif move == '>':
            santaX += 1
        elif move == 'v':
            santaY -= 1

        visitedHouses.incl((santaX, santaY))
    else:
        if move == '^':
            roboY += 1
        elif move == '<':
            roboX -= 1
        elif move == '>':
            roboX += 1
        elif move == 'v':
            roboY -= 1
    
        visitedHouses.incl((roboX, roboY))

echo visitedHouses.len