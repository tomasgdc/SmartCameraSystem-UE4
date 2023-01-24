# k-means clustering
import numpy as np
import csv
from numpy import unique
from numpy import where
from numpy.linalg import norm
from sklearn.datasets import make_classification
#from sklearn.cluster import KMeans
from matplotlib import pyplot


# define dataset
X, _ = make_classification(n_samples=1000, n_features=2, n_informative=2, n_redundant=0, n_clusters_per_class=1, random_state=4)

wtr = csv.writer(open ('kMeansTestData.csv', 'w'), delimiter=',', lineterminator='\n')
for x in X: 
    wtr.writerow (x)