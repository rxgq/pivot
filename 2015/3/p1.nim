import sets, sequtils

let content = readFile("2015/3/puzzle.txt")

var x, y = 0

var visitedHouses = initHashSet[(int, int)]()

visitedHouses.incl((x, y))

for move in content:
    if move == '^':
        y += 1
    elif move == '<':
        x -= 1
    elif move == '>':
        x += 1
    elif move == 'v':
        y -= 1
    
    visitedHouses.incl((x, y))

echo visitedHouses.len