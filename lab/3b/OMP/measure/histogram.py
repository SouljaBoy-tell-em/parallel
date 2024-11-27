import matplotlib.pyplot as plt

ethalon_data = []
par_data = []

with open('ethalon_dataset_3b.txt') as file:
    for line in file:
        ethalon_data.append([float(x) for x in line.split()])

with open('par_dataset_3b.txt') as file:
    for line in file:
        par_data.append([float(x) for x in line.split()])

# best_ethalon_data = min(min(ethalon_data))

amount = []
S = []
for i in range(0, len(par_data[0]), 1):
    amount.append(i)
    S.append(ethalon_data[0][i] / par_data[0][i])
    print(f'THREADS: {i + 1} ->', ethalon_data[0][i] / ((i + 1) * par_data[0][i]))

plt.grid()
plt.plot(amount, S)
plt.show()