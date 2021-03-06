from numpy import genfromtxt
import matplotlib.pyplot as plt
import glob
import numpy as np

for results_csv in glob.glob('*.csv'):
  with open(results_csv, "r") as csv_file:

    data = genfromtxt(csv_file, delimiter=',')

    print(results_csv)

    alignment = data[:, 0]
    copy_bytes = data[:, 1]
    elapsed = data[:, 2]
    
    plt.clf()
    plt.suptitle(results_csv)
    plt.scatter(copy_bytes, copy_bytes / elapsed, s=0.2)
    plt.show()

