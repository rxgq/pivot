import strutils

var total = 0
for line in lines "2015/2/puzzle.text":
    let values = line.split('x')

    let l = parseInt(values[0])
    let w = parseInt(values[1])
    let h = parseInt(values[2])

    let smallestSide = min(@[(2*l)+(2*w), (2*w)+(2*h), (2*h)+(2*l)])
    let ribbonLength = l * w * h

    total += (smallestSide + ribbonLength)

echo total
