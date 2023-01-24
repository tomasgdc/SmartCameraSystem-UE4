# k-means clustering
import numpy as np
import csv
from numpy import unique
from numpy import where
from numpy.linalg import norm
from sklearn.datasets import make_classification
#from sklearn.cluster import KMeans
from matplotlib import pyplot
import seaborn as sns
import pandas as pd


df = pd.read_csv("kMeansTestOutData.csv")

sns.scatterplot(x=df.x, y=df.y, 
                hue=df.c, 
                palette=sns.color_palette("hls", n_colors=5))

pyplot.xlabel("Annual income (k$)")
pyplot.ylabel("Spending Score (1-100)")
pyplot.title("Clustered: spending (y) vs income (x)")
pyplot.show()