import matplotlib.pyplot as plt

files = ["l1.txt", "l2.txt", "l3.txt"]

colors = ["red", "blue", "green"]
labels = ["L1 Cache", "L2 Cache", "L3 Cache"]

for i, filename in enumerate(files):
    x = []  # количество фрагментов (1-32)
    y = []  # время чтения элемента

    with open(filename, "r") as file:
        for line in file:
            fragment_count, access_time = line.strip().split()
            x.append(int(fragment_count))
            y.append(int(access_time))

    # Построение графика для текущего файла
    plt.plot(x, y, label=labels[i], color=colors[i], marker='o')

plt.xlabel("Количество фрагментов")
plt.ylabel("Время чтения элемента (в тактах)")
plt.title("Зависимость времени чтения от количества фрагментов")
plt.legend()
plt.grid(True)
plt.show()
