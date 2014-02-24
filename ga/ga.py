#!/usr/bin/python
import array, random
from fitness import eval_individual
from deap import creator, base, tools, algorithms

NUM_WEIGHTS = 9
toolbox = base.Toolbox()

def create_population(popsize):
    creator.create("FitnessMin", base.Fitness, weights=(-1.0,))
    creator.create("Individual", array.array, typecode='H', fitness=creator.FitnessMin)

    toolbox.register("attr_int", random.choice, range(100))
    toolbox.register("individual", tools.initRepeat, creator.Individual, toolbox.attr_int, NUM_WEIGHTS)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    toolbox.register("evaluate", eval_individual)
    toolbox.register("mate", tools.cxTwoPoints)
    toolbox.register("mutate", tools.mutUniformInt, low=0, up=150, indpb=0.10)
    toolbox.register("select", tools.selBest, k=1)

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
    popsize = 5
    population = create_population(popsize)
#    ind1 = toolbox.individual()
#    ind1.fitness.values = toolbox.evaluate(ind1)
#    print(str(ind1) + ", fitness: " + str(ind1.fitness.values))
    run_evolution(population, 30, open("/home/hvatum/ga-results", "a"))

main()
