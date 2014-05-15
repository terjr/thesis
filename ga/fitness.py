#!/usr/bin/python3

import ast
import sys
import os
import subprocess

from math import sqrt, floor

#import queue
#import threading


def distance(graph1, graph2):
    diff = 0
    for i in range( min(len(graph1), len(graph2))):
        diff += (graph1[i] - graph2[i])**2
    return sqrt(diff / min(len(graph1), len(graph2)));


def align(graph1, graph2):
    if len(graph1) > len(graph2):
        shortest = graph2
        longest = graph1
    else:
        shortest = graph1
        longest = graph2

    tests = len(longest) - len(shortest)
    least_diff = sys.maxsize
    best = 0
    for i in range(tests):
        dist = distance(shortest, longest[i:])
        if dist < least_diff:
            least_diff = dist
            best = i
#    print("Best alignment at idx "+str(best)+" with diff of "+str(least_diff))
    return least_diff


def read_graph(filename):
    with open(filename) as f:
        return [int(number) for number in [line[line.find(' '):-1].strip() for line in f] if number.isdigit()]

def write_weights_file(weights, filename):
    f = open(filename, 'w+')
    f.write('\n'.join(['%s %s' % (key,value) for (key,value) in weights.items()]) + '\n')
    f.close()

best = 10000000
def eval_individual(weights):
    global best
    fitness = 0
    for test in ['trend-trend', 'trend-submul', 'sha2-sha2', 'pi-pi']:
        score = run_test(weights, test)
        fitness += score*score
    fitness = sqrt( fitness / 3 )
    if fitness < best:
        best = fitness
        print('ind = {' + ', '.join(["'%s':%d" % (key, value) for (key,value) in sorted(weights.items(), key=lambda t: t[0])]) + '}')
        print("Fitness = "+str(fitness)+"\n")
    return (fitness,)

def read_input_matrix(tf):
    f = open(tf)
    a = f.readline()
    bucket_size = ast.literal_eval(a)
    b = f.readline()
    return [a, ast.literal_eval(b)]

# Data = { bucket size, stats-map }
def apply_weights(data, weights):
    results = []
    normalize = (int(data[0])/500)
    for d in data[1]:
        value = 0
        numEvents = 0
        for weight in weights:
            value += d[weight]*weights[weight]
            numEvents += d[weight]

        ticksInCycle = floor(1000000/1700)
        idleTicks = int(int(data[0])/ticksInCycle) - numEvents;
        if idleTicks > 0:
            value +=  weights["Idle"]*idleTicks;

        results.append(floor(value/normalize) + weights['Static'])
    return results

def assure_input_matrix(test, weights, buckets):
    tf = "/tmp/" + test
    weightfile = "/tmp/weights"
    prog = ["../pet/pet", "../workloads/m5out-stable/" + test + "/trace.out", "-s", "-v", "-b", str(buckets), "-f", "plain", "-O", tf, "-w", weightfile]
    if not os.path.isfile(tf):
        write_weights_file(weights, weightfile)
        print(" ".join(prog)+"\n")
        os.system(" ".join(prog))
    return tf


def run_test(weights, test):
    hw = "../powerlogs/root/m5bins/" + test.replace("-", "/") + "/pet-log-cut";
#    print("Diffing results with "+hw)
    buckets = sum(1 for line in open(hw))
    tf = assure_input_matrix(test, weights, buckets)
    event_data = read_input_matrix(tf)

    return round(align(apply_weights(event_data, weights), read_graph(hw))*100/buckets)
