let content = readFile("2015/1/puzzle.txt")

var floor = 0
for c in content:
    if c == '(':
        floor += 1
    else:
        floor -= 1

echo floor