import matplotlib.pyplot as plt

average_times_forward_sizes = []
average_times_forward = []
with open('forward_result.txt') as f:
    for line in f:
        t = line.split()
        average_times_forward_sizes.append(t[0])
        average_times_forward.append(t[1])

average_times_reverse_sizes = []
average_times_reverse = []
with open('reverse_result.txt') as f:
    for line in f:
        t = line.split()
        average_times_reverse_sizes.append(t[0])
        average_times_reverse.append(t[1])

average_times_random_sizes = []
average_times_random = []
with open('random_result.txt') as f:
    for line in f:
        t = line.split()
        average_times_random_sizes.append(t[0])
        average_times_random.append(t[1])

# Построение графика
plt.figure(figsize=(12, 7))
plt.plot(average_times_forward_sizes, average_times_forward, marker='o', color='blue', label="Forward Mode")
plt.plot(average_times_reverse_sizes, average_times_reverse, marker='x', color='red', label="Reverse Mode")
plt.plot(average_times_random_sizes, average_times_random, marker='s', color='green', label="Random Mode")

cache_transition = 393216
plt.axvline(x=cache_transition, color='purple', linestyle='-.', label="L1 to L2 Cache Transition")

cache_transition2 = 10485760
plt.axvline(x=cache_transition2, color='green', linestyle='--', label="L2 to L3 Cache Transition")

plt.xscale('log')  # Логарифмическая шкала для оси X
plt.xlabel("Array Size (bytes)")
plt.ylabel("Average Time per Element (ns)")
plt.title("Average Access Time per Element in Forward and Reverse Modes")
plt.legend()
plt.grid(True, which="both", linestyle="--", linewidth=0.5)

# Показать график
# plt.gca().invert_yaxis()

plt.show()
