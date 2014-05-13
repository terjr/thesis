#!/usr/bin/python
import array, random
from math import ceil,floor,sqrt
from fitness import eval_individual
from deap import creator, base, tools, algorithms
import os

from operator import attrgetter

NUM_WEIGHTS = 9
toolbox = base.Toolbox()

def mutate_weights(individual, indpb):
    degree = (individual.fitness.values[0])**1.4 + 100
    if random.random() < sqrt(indpb):
        degree = degree * 3
        indpb = 0.7
    #print("Degree is " + str(degree))
    for (key, value) in individual.items():
        if random.random() < indpb:
            individual[key] = floor(value + ((random.random()-0.5) * degree))
            if individual[key] < 0:
                    individual[key] = -individual[key]
    return check_individual(individual),

def swap_if_greater(d, i1, i2):
    if d[i1] > d[i2]:
        d[i1],d[i2] = d[i2],d[i1]

def check_individual(ind):
#    swap_if_greater(ind, 'L1IR', 'PhysR')
#    swap_if_greater(ind, 'L1IW', 'PhysW')
#    swap_if_greater(ind, 'L1DR', 'PhysR')
#    swap_if_greater(ind, 'L1DW', 'PhysW')
#    swap_if_greater(ind, 'L2R', 'PhysR')
#    swap_if_greater(ind, 'L2W', 'PhysW')

    swap_if_greater(ind, 'L1IR', 'L2R')
    swap_if_greater(ind, 'L1IW', 'L2W')
    swap_if_greater(ind, 'L1DR', 'L2R')
    swap_if_greater(ind, 'L1DW', 'L2W')

    ind['PhysR'] = 0
    ind['PhysW'] = 0

    return ind

def create_individual():
    f = lambda: ceil(random.random() * 1000);
    ind = {}
    ind['IntAlu'] = f()
    ind['IntMult'] = f()
    ind['MemRead'] = f()
    ind['MemWrite'] =f()
    ind['SimdFloatMisc'] = f()*2
    ind['L1IR'] = f()
    ind['L1IW'] = floor(ind['L1IR']+((random.random()-0.5)*10))
    ind['L1DR'] = f()
    ind['L1DW'] = floor(ind['L1DR']+((random.random()-0.5)*10))

    ind['L2R'] = f()*2
    ind['L2W'] = f()*2
    ind['PhysR'] = 0
    ind['PhysW'] = 0
    ind['Static'] = floor(96*(random.random()+0.5))
    ind['Idle'] = floor(50*(random.random()+0.5))

    ind = check_individual(ind)

    return ind;


def create_population(popsize):
    creator.create("FitnessMin", base.Fitness, weights=(-1.0,))
    creator.create("Individual", dict, fitness=creator.FitnessMin)

    toolbox.register("attr_int", random.choice, range(100))
    toolbox.register("individual", tools.initIterate, creator.Individual, create_individual)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    toolbox.register("evaluate", eval_individual)
    toolbox.register("mate", tools.cxTwoPoints)
    toolbox.register("mutate", mutate_weights, indpb=0.30)
    toolbox.register("select", tools.selBest)

    population = toolbox.population(n=popsize)
    return population

def sortmap(d):
    return ['%s %d' % (key, value) for (key,value) in sorted(d.items(), key=lambda t: t[0])]

def run_evolution(population, ngen, file=None):
    cxpb, mutpb = 0.0, 1

    if not file is None:
        file.write("\nGeneration 0\n")

    fitnesses = toolbox.map(toolbox.evaluate, population)
    for ind, fit in zip(population, fitnesses):
        ind.fitness.values = fit
        if not file is None:
            file.write(str(sortmap(ind)) + ", fitness: " + "{:,}".format(fit[0]) + "\n")
            file.flush()

    population = toolbox.select(population, 1)

    for i in range(ngen):
        file.write("\nGeneration " + str(i+1) + "\n")
        offspring = algorithms.varOr(population, toolbox, 4, cxpb, mutpb)

        fitnesses = toolbox.map(toolbox.evaluate, offspring)
        for ind, fit in zip(offspring, fitnesses):
            ind.fitness.values = fit
            if not file is None:
                file.write(str(sortmap(ind)) + ", fitness: " + "{:,}".format(fit[0]) + "\n")
                file.flush()

        np = toolbox.select(offspring, 1)
        if np[0].fitness.values[0] <= population[0].fitness.values[0]:
            population = np
    return population

def main():
    popsize = 12
    population = create_population(popsize)

    ind1 = toolbox.individual()
#    ind1.update({'Idle':60, 'IntAlu':140, 'IntMult':618, 'L1DR':40, 'L1DW':60, 'L1IR':30, 'L1IW':60, 'L2R':500, 'L2W':800, 'MemRead':140, 'MemWrite':160, 'PhysR':0, 'PhysW':0, 'SimdFloatMisc':1364, 'Static':60})
#   Idle 302 IntAlu 268 IntMult 1123 L1DR 23 L1DW 846 L1IR 23 L1IW 129 L2R 1631 L2W 1588 MemRead 12 MemWrite 600 PhysR 0 PhysW 0 SimdFloatMisc 1367 Static 8
#    ind1.update({'Idle':216, 'IntAlu':290, 'IntMult':1111, 'L1DR':11, 'L1DW':814, 'L1IR':11, 'L1IW':117, 'L2R':1619, 'L2W':1516, 'MemRead':0, 'MemWrite':588, 'PhysR':0, 'PhysW':0, 'SimdFloatMisc':1315, 'Static':20})
#    print(ind1)
#    population = [ind1] + population
#
#    ind1 = toolbox.individual()
#   ind1.update({'IntAlu':190, 'IntMult':1437, 'L1DR':202, 'L1DW':310, 'L1IR':201, 'L1IW':511, 'L2R':1150, 'L2W':1088, 'MemRead':163, 'MemWrite':17, 'PhysR':2718, 'PhysW':2765, 'SimdFloatMisc':1294, 'Static':159, 'Idle':70})
#    ind1.update({'Idle':97, 'IntAlu':93, 'IntMult':1042, 'L1DR':35, 'L1DW':101, 'L1IR':221, 'L1IW':233, 'L2R':803, 'L2W':1896, 'MemRead':174, 'MemWrite':621, 'PhysR':0, 'PhysW':0, 'SimdFloatMisc':1684, 'Static':130})
#    print(ind1)
#    population = [ind1] + population


    home = os.environ.get('HOME')
    run_evolution(population, 600, open(home + "/ga-results", "a"))

main()
