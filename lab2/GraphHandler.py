import matplotlib.pyplot as plt

n_value = 750000000

times_o0 = 16.55
times_o1 = 15.97
times_o2 = 15.87
times_o3 = 15.89
times_os = 15.74
times_ofast = 15.61
times_og = 16.05

# уровни оптимизации
optimization_levels = ['-O0', '-O1', '-O2', '-O3', '-Os', '-Ofast', '-Og']
times = [times_o0, times_o1, times_o2, times_o3, times_os, times_ofast, times_og]

plt.figure(figsize=(10, 5))

# график
plt.plot(optimization_levels, times, 'o-', color='b', label=f'N = {n_value}')

plt.xlabel('Optimization Levels')
plt.ylabel('Time taken (seconds)')

plt.legend()
plt.grid(True)

plt.show()
# plt.savefig('plot.png')