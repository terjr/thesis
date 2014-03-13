#!/usr/bin/python
import array, random
from math import ceil,floor
from fitness import eval_individual
from deap import creator, base, tools, algorithms

from operator import attrgetter

NUM_WEIGHTS = 9
toolbox = base.Toolbox()

def mutate_weights(individual, indpb):
    degree = (individual.fitness.values[0] / 3000) + 100
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
    swap_if_greater(ind, 'L1R', 'PhysR')
    swap_if_greater(ind, 'L1W', 'PhysW')
    swap_if_greater(ind, 'L2R', 'PhysR')
    swap_if_greater(ind, 'L2W', 'PhysW')

    swap_if_greater(ind, 'L1R', 'L2R')
    swap_if_greater(ind, 'L1W', 'L2W')

    return ind

def create_individual():
    f = lambda: ceil(random.random() * 1000);
    ind = {}
    ind['IntAlu'] = f()
    ind['IntMult'] = f()
    ind['MemRead'] = f()
    ind['MemWrite'] =f()
    ind['SimdFloatMisc'] = f()*2
    ind['L1R'] = f()
    ind['L1W'] = ind['L1R']+((random.random()+0.5)*10)
    ind['L2R'] = f()*2
    ind['L2W'] = f()*2
    ind['PhysR'] = f()*5
    ind['PhysW'] = f()*5
    ind['Static'] = f()

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
    popsize = 0
    population = create_population(popsize)

    #has good fitness: ['IntAlu 22', 'IntMult 129', 'L1D 3', 'L1I 0', 'L2 3', 'MemRead 26', 'MemWrite 19', 'Phys 299', 'SimdFloatMisc 136']
    #['IntAlu 228', 'IntMult 1097', 'L1D 232', 'L1I 9', 'L2 1828', 'MemRead 217', 'MemWrite 84', 'Phys 4452', 'SimdFloatMisc 1334', 'Static 36']
    #['IntAlu 179', 'IntMult 918', 'L1D 273', 'L1I 191', 'L2 1647', 'MemRead 124', 'MemWrite 9', 'Phys 3697', 'SimdFloatMisc 1542', 'Static 70']
    #['IntAlu 176', 'IntMult 931', 'L1D 297', 'L1I 207', 'L2 1431', 'MemRead 69', 'MemWrite 63', 'Phys 3520', 'SimdFloatMisc 1538', 'Static 69'], fitness: 1,710
    #['IntAlu 176', 'IntMult 1021', 'L1D 303', 'L1I 212', 'L2 1149', 'MemRead 84', 'MemWrite 33', 'Phys 2589', 'SimdFloatMisc 1446', 'Static 68'], fitness: 1,160
    #['IntAlu 176' 'IntMult 1021' 'L1D 303' 'L1I 212' 'L2 1149' 'MemRead 84' 'MemWrite 33' 'Phys 2599' 'SimdFloatMisc 1457' 'Static 68'] fitness: 1160
    #['IntAlu 176', 'IntMult 999', 'L1R 243', 'L1W 401', 'L2R 1137', 'L2W 1149', 'MemRead 86', 'MemWrite 34', 'PhysR 2580', 'PhysW 2691', 'SimdFloatMisc 1414', 'Static 68'], fitness: 2,802
    ind1 = toolbox.individual()
    ind1['IntAlu'] = 176
    ind1['IntMult'] = 999
    ind1['L1R'] = 243
    ind1['L1W'] = 363
    ind1['L2R'] = 1122
    ind1['L2W'] = 1149
    ind1['MemRead'] = 86
    ind1['MemWrite'] = 34
    ind1['PhysR'] = 2580
    ind1['PhysW'] = 2647
    ind1['SimdFloatMisc'] = 1414
    ind1['Static'] = 68

    population = [ind1] + population

    run_evolution(population, 600, open("/home/hvatum/ga-results", "a"))

main()
