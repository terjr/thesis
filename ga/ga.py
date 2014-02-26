#!/usr/bin/python
import array, random
from math import ceil,floor
from fitness import eval_individual
from deap import creator, base, tools, algorithms

from operator import attrgetter

NUM_WEIGHTS = 9
toolbox = base.Toolbox()

def mutate_weights(individual, indpb):
    degree = (individual.fitness.values[0] / 20000) + 10
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

    return ind

def create_individual():
    f = lambda: ceil(random.random() * 150);
    ind = {}
    ind['IntAlu'] = f()
    ind['IntMult'] = f()
    ind['MemRead'] = f()
    ind['MemWrite'] =f()
    ind['SimdFloatMisc'] = f()
    ind['L1I'] = f()
    ind['L1D'] = f()
    ind['L2'] = f()
    ind['Phys'] = f()*3

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
    toolbox.register("mutate", mutate_weights, indpb=0.15)
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
    popsize = 5
    population = create_population(popsize)

    #has good fitness: ['IntAlu 22', 'IntMult 129', 'L1D 3', 'L1I 0', 'L2 3', 'MemRead 26', 'MemWrite 19', 'Phys 299', 'SimdFloatMisc 136']
    ind1 = toolbox.individual()
    ind1['IntAlu'] = 22
    #ind1['IntMult'] = 129
    #ind1['MemRead'] = 26
    #ind1['MemWrite'] = 19
    #ind1['SimdFloatMisc'] = 136
    ind1['L1D'] = 20
    ind1['L1I'] = 20
    ind1['L2'] = 100
    ind1['Phys'] = 299
    population += [ind1]

    run_evolution(population, 200, open("/home/hvatum/ga-results", "a"))

main()
