#!/usr/bin/python
import array, random
from math import ceil,floor
from fitness import eval_individual
from deap import creator, base, tools, algorithms
import os

from operator import attrgetter

NUM_WEIGHTS = 9
toolbox = base.Toolbox()

def mutate_weights(individual, indpb):
    degree = (individual.fitness.values[0])**1.5 + 100
    print("Degree is " + str(degree))
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
    swap_if_greater(ind, 'L1IR', 'PhysR')
    swap_if_greater(ind, 'L1IW', 'PhysW')
    swap_if_greater(ind, 'L1DR', 'PhysR')
    swap_if_greater(ind, 'L1DW', 'PhysW')
    swap_if_greater(ind, 'L2R', 'PhysR')
    swap_if_greater(ind, 'L2W', 'PhysW')

    swap_if_greater(ind, 'L1IR', 'L2R')
    swap_if_greater(ind, 'L1IW', 'L2W')
    swap_if_greater(ind, 'L1DR', 'L2R')
    swap_if_greater(ind, 'L1DW', 'L2W')

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
    ind['PhysR'] = f()*5
    ind['PhysW'] = f()*5
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
    toolbox.register("mutate", mutate_weights, indpb=0.20)
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
    popsize = 2
    population = create_population(popsize)

#    ind1 = toolbox.individual()
#    ind1.update({'IntAlu':138, 'IntMult':1116, 'L1DR':171, 'L1DW':494, 'L1IR':430, 'L1IW':682, 'L2R':928, 'L2W':1960, 'MemRead':267, 'MemWrite':220, 'PhysR':1097, 'PhysW':3810, 'SimdFloatMisc':14, 'Static':96})
#    print(ind1)
#    population = [ind1] + population
#
    ind1 = toolbox.individual()
    ind1.update({'IntAlu':190, 'IntMult':1437, 'L1DR':202, 'L1DW':310, 'L1IR':201, 'L1IW':511, 'L2R':1150, 'L2W':1088, 'MemRead':163, 'MemWrite':17, 'PhysR':2718, 'PhysW':2765, 'SimdFloatMisc':1294, 'Static':159, 'Idle':70})
    print(ind1)
    population = [ind1] + population


    home = os.environ.get('HOME')
    run_evolution(population, 600, open(home + "/ga-results", "a"))

main()
