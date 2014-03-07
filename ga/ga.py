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
    swap_if_greater(ind, 'L1D', 'Phys')
    swap_if_greater(ind, 'L1I', 'Phys')
    swap_if_greater(ind, 'L2', 'Phys')

    swap_if_greater(ind, 'L1D', 'L2')
    swap_if_greater(ind, 'L1I', 'L2')

    if ind['L1I'] / ind['L1D'] > 1.4:
        ind['L1I'] = ind['L1D'] * 1.4
    elif ind['L1I'] / ind['L1D'] < 0.7:
        ind['L1I'] = ind['L1D'] * 0.7

    return ind

def create_individual():
    f = lambda: ceil(random.random() * 1000);
    ind = {}
    ind['IntAlu'] = f()
    ind['IntMult'] = f()
    ind['MemRead'] = f()
    ind['MemWrite'] =f()
    ind['SimdFloatMisc'] = f()*2
    ind['L1I'] = f()
    ind['L1D'] = ind['L1I']+((random.random()+0.5)*10)
    ind['L2'] = f()*2
    ind['Phys'] = f()*5
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

        population = toolbox.select(population + offspring, 1)
    return population

def main():
    popsize = 0
    population = create_population(popsize)

    #has good fitness: ['IntAlu 22', 'IntMult 129', 'L1D 3', 'L1I 0', 'L2 3', 'MemRead 26', 'MemWrite 19', 'Phys 299', 'SimdFloatMisc 136']
    #['IntAlu 228', 'IntMult 1097', 'L1D 232', 'L1I 9', 'L2 1828', 'MemRead 217', 'MemWrite 84', 'Phys 4452', 'SimdFloatMisc 1334', 'Static 36']
    ind1 = toolbox.individual()
    ind1['IntAlu'] = 220
    ind1['IntMult'] = 990
    ind1['MemRead'] = 210
    ind1['MemWrite'] = 210
    ind1['SimdFloatMisc'] = 1360
    ind1['L1D'] = 130
    ind1['L1I'] = 130
    ind1['L2'] = 1700
    ind1['Phys'] = 4000
    ind1['Static'] = 100

    population = [ind1] + population

    run_evolution(population, 200, open("/home/hvatum/ga-results", "a"))

main()
