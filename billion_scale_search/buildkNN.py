import faiss
import numpy as np 
import random

filename = "/home/y/yujianfu/similarity_search/datasets/ANN_SIFT1B/1milliard.p1.siftbin"
x = np.memmap(filename, dtype='uint8', mode='r')
d = x[:4].view('int32')[0]
x = x.reshape(-1, d + 4)[:, 4:]
dataset_size = x.shape[0]
k = 100
niter = 100
proportion = 0.1
#index = random.sample(range(0, x.shape[0]), int(proportion * x.shape[0]))


for i in range(int(1/proportion)):
    processing_subset = np.ascontiguousarray(x[int(dataset_size*proportion*i) : int(dataset_size*proportion*(i+1)), :])
    kmeans = faiss.Kmeans(d, k, niter = niter, verbose = True)
    kmeans.train(processing_subset)
    print(kmeans.obj)
    print(kmeans.centroids)


