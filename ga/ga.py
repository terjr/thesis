#!/usr/bin/python
import array, random
from math import ceil,floor
from fitness import eval_individual
from deap import creator, base, tools, algorithms

NUM_WEIGHTS = 9
toolbox = base.Toolbox()

def mutate_weights(individual, indpb):
    print(individual)
    for (key, value) in individual.items():
        if random.random() < indpb:
            individual[key] = floor(value + ((random.random()-0.5) * 10))
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
    ind['Phys'] = f()

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
    toolbox.register("mutate", mutate_weights, indpb=0.2)
    toolbox.register("select", tools.selBest)

    population = toolbox.population(n=popsize)
    return population



def run_evolution(population, ngen, file=None):
    cxpb, mutpb = 0.0, 1
#    stats = tools.Statistics()
#    halloffame = tools.HallOfFame(20)
#    final = algorithms.eaMuPlusLambda(pop, toolbox, 1, 4, CXPB, MUTPB, NGEN, stats, halloffame, True)
#    print(halloffame)
#    print(final)
#    return final

    if not file is None:
        file.write("\nGeneration 0\n")

    fitnesses = toolbox.map(toolbox.evaluate, population)
    for ind, fit in zip(population, fitnesses):
        ind.fitness.values = fit
        if not file is None:
            file.write(str(ind) + ", fitness: " + str(fit) + "\n")
            file.flush()

    for i in range(ngen):
        file.write("\nGeneration " + str(i+1) + "\n")
        offspring = algorithms.varOr(population, toolbox, 4, cxpb, mutpb)

        fitnesses = toolbox.map(toolbox.evaluate, offspring)
        for ind, fit in zip(offspring, fitnesses):
            ind.fitness.values = fit
            if not file is None:
                file.write(str(ind) + ", fitness: " + str(fit) + "\n")
                file.flush()
        population = toolbox.select(population + offspring, 1)
    return population

#    toolbox.evaluate(pop)
#    for gen in range(NGEN):
#        if not file is None:
#            file.write("\nGeneration " + str(gen) + "\n")
#        # Select and clone the next generation individuals
#        offspring = map(toolbox.clone, toolbox.select(pop, len(pop)))
#
#        # Apply crossover and mutation on the offspring
#        offspring = algorithms.varAnd(offspring, toolbox, CXPB, MUTPB)
#
#        # Evaluate the individuals with an invalid fitness
#        invalid_ind = [ind for ind in offspring if not ind.fitness.valid]
#        fitnesses = toolbox.map(toolbox.evaluate, invalid_ind)
#        for ind, fit in zip(invalid_ind, fitnesses):
#            ind.fitness.values = fit
#            if not file is None:
#                file.write(str(ind) + ", fitness: " + str(fit) + "\n")
#                file.flush()
#
#        # The population is entirely replaced by the offspring
#        pop[:] = offspring
#    return pop

def main():
    popsize = 1
    population = create_population(popsize)
#    ind1 = toolbox.individual()
#    ind1.fitness.values = toolbox.evaluate(ind1)
#    print(str(ind1) + ", fitness: " + str(ind1.fitness.values))
    run_evolution(population, 10, open("/home/hvatum/ga-results", "a"))

main()
