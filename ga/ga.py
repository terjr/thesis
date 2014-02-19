import array, random
#from fitness import eval_individual
from deap import creator, base, tools, algorithms

NUM_WEIGHTS = 9
toolbox = base.Toolbox()

def create_population():
    creator.create("FitnessMin", base.Fitness, weights=(-1.0,))
    creator.create("Individual", array.array, typecode='H', fitness=creator.FitnessMin)

    toolbox.register("attr_int", random.choice, range(100))
    toolbox.register("individual", tools.initRepeat, creator.Individual, toolbox.attr_int, NUM_WEIGHTS)
    toolbox.register("population", tools.initRepeat, list, toolbox.individual)

    #toolbox.register("evaluate", eval_individual)
    toolbox.register("mate", tools.cxTwoPoints)
    toolbox.register("mutate", tools.mutUniformInt, low=0, up=150, indpb=0.05)
    toolbox.register("select", tools.selTournament, tournsize=5)

    population = toolbox.population(n=300)
    return population

def run_evolution(population, NGEN):
    for gen in range(NGEN):
        offspring = algorithms.varAnd(population, toolbox, cxpb=0.5, mutpb=0.1)
        fits = toolbox.map(toolbox.evaluate, offspring)
        for fit, ind in zip(fits, offspring):
            ind.fitness.values = fit
        population = offspring

def main():
    population = create_population()
    #ind1 = toolbox.individual()
    #print ind1
    #print toolbox.mutate(ind1)[0]
    run_evolution(population, 400)

main()
