#!/usr/bin/python

import os
import sys
import time

thisdir = os.path.dirname(os.path.realpath(__file__))

sys.path.append(thisdir + "/../ga/")

from fitness import apply_weights, assure_input_matrix, read_input_matrix

def read_weights():
    f = open("../pet/weights.conf")
    weights = {}
    for line in f:
        words = line.split()
        try:
            weights[words[0]] = int(words[1])
        except:
            continue
    return weights

for arg in sys.argv:
    if arg == __file__:
        continue
    hw = "../powerlogs/root/m5bins/" + arg.replace("-", "/") + "/pet-log-cut";
#    print("Diffing results with "+hw)
    buckets = sum(1 for line in open(hw))
    weights = read_weights()
    tf = assure_input_matrix(arg, weights, buckets)
    event_data = read_input_matrix(tf)
    i = 0
    for datapoint in apply_weights(event_data, weights):
        print(str(i) + " " + str(datapoint))
        i += 1

