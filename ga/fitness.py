#!/usr/bin/python3

import sys
import os
import subprocess

def distance(graph1, graph2):
    diff = 0
    for i in range( min(len(graph1), len(graph2))):
        diff += abs(graph1[i] - graph2[i])
    return diff;


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


#def read_graph(filename):
#    lines = []
#    for line in open(filename):
#        number = line[line.find(' '):-1].strip()
#        if number.isdigit():
#            lines.append(int(number))
#    return lines;


def write_weights_file(weights, filename):
    f = open(filename, 'w+')
    f.write("IntAlu "+str(weights[0])+"\n")
    f.write("IntMult "+str(weights[1])+"\n")
    f.write("MemRead "+str(weights[2])+"\n")
    f.write("MemWrite "+str(weights[3])+"\n")
    f.write("SimdFloat "+str(weights[4])+"\n")
    f.write("L1I "+str(weights[5])+"\n")
    f.write("L1D "+str(weights[6])+"\n")
    f.write("L2 "+str(weights[7])+"\n")
    f.write("Phys "+str(weights[8])+"\n")
    f.close()


def eval_individual(weights):
    name = ''.join(map(str, weights))
    write_weights_file(weights, "/home/hvatum/NAS/stian/Skole/pest-tmp/"+name+"_weights.conf")
    fitness = run_tests("/home/hvatum/NAS/stian/Skole/pest-tmp/"+name+".output", "/home/hvatum/NAS/stian/Skole/pest-tmp/"+name+"_weights.conf")
    print("Fitness = "+str(fitness)+"\n")
    return (fitness,)


def run_test(output, weightfile, test):
    tf = output + "-" + test
    hw = "/home/hvatum/NAS/stian/Skole/pest-tmp/" + test + "-power";
    buckets = sum(1 for line in open(hw))

    prog = ["/home/hvatum/Skole/thesis/pest/pest", "/home/hvatum/Skole/thesis/workloads/m5out/" + test + "/trace.out", "-b", str(buckets), "-f", "plain", "-o", tf, "-w", weightfile]
    if not os.path.isfile(tf):
        print(" ".join(prog)+"\n")
        os.system(" ".join(prog))
    return align(read_graph(tf), read_graph(hw))



def run_tests(output, weightfile):
    return run_test(output, weightfile, "pi-pi") + run_test(output, weightfile, "trend-trend")

#print(eval_individual([1,2,3,4,5,6,7,8,9]))
#print(eval_individual([22,30,15,40,30,3,3,30,90]))
#idx = align(read_graph(sys.argv[1]), read_graph(sys.argv[2]))

