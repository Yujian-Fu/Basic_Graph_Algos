import faiss
import numpy as np 
import random
from sklearn.cluster import KMeans


filename = "/home/y/yujianfu/similarity_search/datasets/ANN_SIFT1B/1milliard.p1.siftbin"
x = np.memmap(filename, dtype='uint8', mode='r')
d = x[:4].view('int32')[0]
x = x.reshape(-1, d + 4)[:, 4:]
dataset_size = x.shape[0]
print("The dataset size is ", dataset_size)
k = 100
niter = 100
proportion = 0.01
#index = random.sample(range(0, x.shape[0]), int(proportion * x.shape[0]))


for i in range(int(1/proportion)):
    print(i, " th iteration")
    #processing_subset = np.ascontiguousarray(x[int(dataset_size*proportion*i) : int(dataset_size*proportion*(i+1)), :].astype('float32'))
    processing_subset = np.ascontiguousarray(x[int(dataset_size*proportion*i) : int(dataset_size*proportion*(i+1)), :])
    print("Finish data loading, start clustering")
    #kmeans = faiss.Kmeans(d, k, niter = niter, verbose = True, gpu=True)
    #kmeans.train(processing_subset)
    #print(kmeans.obj)
    #print(kmeans.centroids)
    kmeans = KMeans(n_clusters=100, n_init=100, max_iter=300,
    verbose=True, n_jobs=-1).fit(processing_subset)
    print(kmeans.labels_)
    print(kmeans.cluster_centers_)
    

