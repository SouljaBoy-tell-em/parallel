import matplotlib.pyplot as plt

data = []
with open('lab.txt') as file:
    for line in file:
        data.append([float(x) for x in line.split()])

b = 0.0
for i in range(0, 40, 2):
    b+=0.1
    plt.figure()
    plt.grid()
    plt.title(f'b = {b}')
    plt.plot(data[i], data[i+1])
    plt.show()