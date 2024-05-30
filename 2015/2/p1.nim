import strutils

var total = 0
for line in lines "2015/2/puzzle.text":
    let values = line.split('x')

    let l = parseInt(values[0])
    let w = parseInt(values[1])
    let h = parseInt(values[2])

    let area = (2 * l * w) + (2 * w * h) + (2 * h * l)
    let smallestSide = min(@[(l*w), (w*h), (h*l)])

    total += (area + smallestSide)

echo total
