import numpy as np
import matplotlib.pyplot as plt

if __name__ == '__main__':
    string = open("D:\_Programming\C++\OGKG_Lab\OGKG_Lab\out1.txt", 'r').read()
    x = list(map(float, string.split()[::2]))
    y = list(map(float, string.split()[1::2]))
    str2 = open("D:\_Programming\C++\OGKG_Lab\OGKG_Lab\out2.txt", 'r').read()
    x1 = list(map(float, string.split()[::2]))
    y1 = list(map(float, string.split()[1::2]))
    print(len(x1),len(y1))
    plt.plot(x, y)
    plt.fill_between(x, y, color="grey")
    plt.plot(x1, y1, 'ro')
    plt.show()

