#!/usr/bin/python3

import sys
import os
import subprocess

from math import sqrt

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
    print("Best alignment at idx "+str(best)+" with diff of "+str(least_diff))
    return least_diff


def read_graph(filename):
    with open(filename) as f:
        return [int(number) for number in [line[line.find(' '):-1].strip() for line in f] if number.isdigit()]

def write_weights_file(weights, filename):
    f = open(filename, 'w+')
    f.write('\n'.join(['%s %s' % (key,value) for (key,value) in weights.items()]) + '\n')
    f.close()

def eval_individual(weights):
    print('ind = {' + ', '.join(["'%s':%d" % (key, value) for (key,value) in sorted(weights.items(), key=lambda t: t[0])]) + '}')
    name = ''.join(['%04d' % (value) for (key,value) in sorted(weights.items(), key=lambda t: t[0])])
    write_weights_file(weights, "/tmp/"+name+"_weights.conf")
    fitness = run_tests("/tmp/"+name+".output", "/tmp/"+name+"_weights.conf")
    print("Fitness = "+str(fitness)+"\n")
    return (fitness,)

def run_test(output, weightfile, test):
    tf = output + "-" + test
    hw = "../powerlogs/root/m5bins/" + test.replace("-", "/") + "/pet-log-cut";
    print("Diffing results "+tf+" with "+hw)
    buckets = sum(1 for line in open(hw))
    buckets = buckets

    prog = ["../pet/pet", "../workloads/m5out-stable/" + test + "/trace.out", "-v", "-b", str(buckets), "-f", "plain", "-o", tf, "-w", weightfile]
    if not os.path.isfile(tf):
        print(" ".join(prog)+"\n")
        os.system(" ".join(prog))
    return round(align(read_graph(tf), read_graph(hw))*100/buckets)

def run_tests(output, weightfile):
#    q = queue.Queue()
#    tests = ["pi-pi", "trend-trend", "sha2-sha2"]
#    threads = []
#    for test in tests:
#        threads.append(threading.Thread(target=run_test, args=(output, weightfile, test, q)))
#
#    for thread in threads:
#        thread.start()
#    for thread in threads:
#        thread.join()
#    
#    fitness = 0
#    while not q.empty():
#        fitness = fitness + q.get()
#    return fitness
    fitness = 0
    for test in ['trend-trend', 'trend-submul', 'sha2-sha2', 'pi-pi']:
        score = run_test(output, weightfile, test)
        fitness += score*score
#    fitness += ( run_test(output, weightfile, 'pi-pi') * 2 )
    return sqrt( fitness / 3 )
