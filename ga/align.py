#!/usr/bin/python3

import sys

def distance(graph1, graph2):
    print(min(len(graph1), len(graph2) ))
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
    print("Best alignment at idx "+str(i))
    return best





def read_graph(filename):
    lines = []
    for line in open(filename):
        number = line[line.find(' '):-1].strip()
        if number.isdigit():
            lines.append(int(number))
    return lines;




#

idx = align(read_graph("file1"), read_graph("file2"))
print(idx)


