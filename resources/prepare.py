

with open('log.txt', 'r') as f:

    for line in f:
        data = line.split("-")
        print(data[1], data[2], data[3])


