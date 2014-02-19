#!/usr/bin/python3

import sys
from subprocess import call

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
#    print("Best alignment at idx "+str(best)+" with diff of "+str(least_diff))
    return least_diff


def read_graph(filename):
    lines = []
    for line in open(filename):
        number = line[line.find(' '):-1].strip()
        if number.isdigit():
            lines.append(int(number))
    return lines;


def write_weights_file(weights, filename):
    f = open(filename)
    f.write("IntAlu "+str(weights[0])+"\n")
    f.write("IntMult"+str(weights[1])+"\n")
    f.write("MemRead"+str(weights[2])+"\n")
    f.write("MemWrite"+str(weights[3])+"\n")
    f.write("SimdFloat"+str(weights[4])+"\n")
    f.write("L1I"+str(weights[5])+"\n")
    f.write("L1D "+str(weights[6])+"\n")
    f.write("L2 "+str(weights[7])+"\n")
    f.write("Phys "+str(weights[8])+"\n")
    f.close()


def eval_individual(weights):
    name = str(weights)
    write_weights_file("/tmp/"+name+"_weights.conf")
    return run_tests("/tmp/"+name+".output", "/tmp/"+name+"_weights.conf")


def run_tests(output, weightfile):
    call(["/home/hvatum/Skole/thesis/pest/pest", "/home/hvatum/Skole/thesis/workloads/m5out/misc/pi/trace.out", "-o", output, "-w", weightfile])
    return align(read_graph(output), read_graph("/tmp/pi-power"))

#idx = align(read_graph(sys.argv[1]), read_graph(sys.argv[2]))

