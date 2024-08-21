import matplotlib.pyplot as plt

# Data
n_values = [4000000000, 4500000000, 5000000000]

times_o0 = [35.1887, 39.8341, 43.9038]
times_o1 = [8.04768, 9.01514, 9.90664]
times_o2 = [7.84239, 8.87028, 9.99184]
times_o3 = [7.96013, 8.82066, 9.77772]
times_os = [8.39481, 9.47931, 10.3841]
times_ofast = [7.8265, 8.91898, 10.0095]
times_og = [7.99961, 8.88734, 9.85844]

# Plotting
plt.figure(figsize=(12, 6))

plt.plot(n_values, times_o0, 'r-o', label='-O0')
plt.plot(n_values, times_o1, 'g-o', label='-O1')
plt.plot(n_values, times_o2, 'b-o', label='-O2')
plt.plot(n_values, times_o3, 'y-o', label='-O3')
plt.plot(n_values, times_os, 'c-o', label='-Os')
plt.plot(n_values, times_ofast, 'm-o', label='-Ofast')
plt.plot(n_values, times_og, 'k-o', label='-Og')

plt.xlabel('N value')
plt.ylabel('Time taken (seconds)')
plt.title('Execution Time vs Optimization Levels and N Values')
plt.legend()
plt.grid(True)

# Show plot
plt.show()
