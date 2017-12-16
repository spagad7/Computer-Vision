import numpy as np

f = open("features_SIFT.txt")
nums_str = []

for line in f:
    nums_str.append(line.split(","))

nums = []
for line in nums_str:
    nums.append([float(i) for i in line])

for feature in nums:
    print(np.linalg.norm(feature))
