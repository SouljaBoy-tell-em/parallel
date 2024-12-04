import matplotlib.pyplot as plt
import sys

data = []
with open('lab.txt') as file:
    for line in file:
        data.append([float(x) for x in line.split()])

ethalon_time = []
with open('times.txt') as file:
    for line in file:
        ethalon_time.append([float(x) for x in line.split()])

par_time = []
with open('times_par.txt') as file:
    for line in file:
        par_time.append([float(x) for x in line.split()])

b = 0.0
for i in range(0, 20, 2):
    b+=0.1
    plt.figure()
    plt.grid()
    plt.title(f'b = {b}')
    plt.plot(data[i], data[i+1])
    plt.show()


print('acceleration:', min(min(ethalon_time)) / min(min(par_time)))
print('impact:', min(min(ethalon_time)) / (float(sys.argv[1]) * min(min(par_time))))