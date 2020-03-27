import faiss
import numpy as np 
import random
import datetime

filename = "/home/y/yujianfu/similarity_search/datasets/ANN_SIFT1B/1milliard.p1.siftbin"
x = np.memmap(filename, dtype='uint8', mode='r')
d = x[:4].view('int32')[0]
x = x.reshape(-1, d + 4)[:, 4:]

print("The shape of x is ", x.shape)
print("testing random read and sequential read")


print("1000 instances")
starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 1000)
dataset = np.ascontiguousarray(x[index, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds

starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 1000)
dataset = np.ascontiguousarray(x[0:1000, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds

print("10000 instances")
starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 10000)
dataset = np.ascontiguousarray(x[index, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds

starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 10000)
dataset = np.ascontiguousarray(x[0:10000, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds

print("100000 instances")
starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 10000)
dataset = np.ascontiguousarray(x[index, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds

starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 10000)
dataset = np.ascontiguousarray(x[0:10000, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds

print("1000000 instances")
starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 1000000)
dataset = np.ascontiguousarray(x[index, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds

starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 1000000)
dataset = np.ascontiguousarray(x[0:1000000, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds

print("10000000 instances")
starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 10000000)
dataset = np.ascontiguousarray(x[index, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds

starttime = datetime.datetime.now()
index = random.sample(range(0, x.shape[0]), 10000000)
dataset = np.ascontiguousarray(x[0:10000000, :])
endtime = datetime.datetime.now()
print (endtime - starttime).seconds