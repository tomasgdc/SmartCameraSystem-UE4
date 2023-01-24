# k-means clustering
import numpy as np
import csv
from numpy import unique
from numpy import where
from numpy.linalg import norm
from sklearn.datasets import make_classification
from sklearn.cluster import KMeans
from matplotlib import pyplot
import seaborn as sns
import pandas as pd
import itertools as IT

with open('kMeansTestData.csv', newline='') as csvfile:
    x = list(csv.reader(csvfile))
    csvData = np.array(x)

# define the model
model = KMeans(n_clusters=5)

# fit the model
model.fit(csvData)

# assign a cluster to each example
predict = model.predict(csvData)


wtr = csv.writer(open ('kMeansTestPythonOutData.csv', 'w'), delimiter=',', lineterminator='\n')
wtr.writerow(['x', 'y', 'c'])
for data, cluster in zip(csvData, predict): 
    wtr.writerow([data[0], data[1], cluster])

#read it
df = pd.read_csv("kMeansTestPythonOutData.csv")

sns.scatterplot(x=df.x, y=df.y, 
                hue=df.c, 
                palette=sns.color_palette("hls", n_colors=5))

pyplot.xlabel("Annual income (k$)")
pyplot.ylabel("Spending Score (1-100)")
pyplot.title("Clustered: spending (y) vs income (x)")
pyplot.show()