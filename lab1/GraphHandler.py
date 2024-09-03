import matplotlib.pyplot as plt

optimization_levels = ['-O0', '-O1', '-O2', '-O3', '-Os']
times_o0 = [14.423, 14.2111, 14.1823, 14.1113, 14.1713, 14.2198]
times_o1 = [13.9702, 13.9063, 13.9273, 13.9084, 13.8982, 13.9221]
times_o2 = [13.9923, 13.8732, 13.8724, 13.8527, 13.8729, 13.8927]
times_o3 = [13.9162, 13.8699, 13.8782, 13.8897, 13.9842, 13.9077]
times_os = [13.7664, 13.8066, 14.025, 13.2114, 13.7947, 13.9208]

# последних значения времени (средние) для каждого уровня оптимизации
last_times = [
    times_o0[-1],
    times_o1[-1],
    times_o2[-1],
    times_o3[-1],
    times_os[-1]
]

plt.figure(figsize=(12, 6))

plt.plot(optimization_levels, last_times, 'o-', color='skyblue')

plt.xlabel('Optimization Level')
plt.ylabel('Last Time Taken (seconds)')
plt.title('Last Execution Time vs Optimization Levels')
plt.grid(True)

plt.show()
