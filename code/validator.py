import argparse, re

class Vertex:
    def __init__(self, id : int, x : int, y : int):
        self.id = id
        self.x = x
        self.y = y
    
    def __repr__(self):
        return f'{self.id}:({self.x},{self.y})'

class Edge:
    def __init__(self, start : int, end : int, capacity : int):
        self.start = start
        self.end = end
        self.capacity = capacity
        self.load = 0
    
    def __repr__(self):
        return f'({self.start},{self.end},{self.capacity}, {self.load}, {self.cost})'

class Car:
    def __init__(self, src : int, dest : int):
        self.src = src
        self.dest = dest
        self.path = []
    
    def __repr__(self):
        return f'({self.src},{self.dest})'


def get_edge(src: list[Edge], dest : int) -> Edge:
    for edge in src:
        if edge.end == dest:
            return edge
    return None

def get_cost(e : Edge, vertices : list[Vertex]):
    start = vertices[e.start]
    end = vertices[e.end]
    return abs(end.x - start.x) + abs(end.y - start.y) 


def parse_problem(file_name : str):
    graphFile = open(file_name, "r").read().split("\n")

    vertices = []
    edges = []
    cars = []

    # Read Data from files
    e = 0
    for i in range(len(graphFile)):
        if graphFile[i] == "EDGES":
            e = 1
            continue
        elif graphFile[i] == "CARS":
            e = 2
            continue

        if e < 1:
            v = graphFile[i].split(":")[1].split(",")
            vertices.append(Vertex(i, int(v[0].strip("()")), int(v[1].strip("()"))))
        elif e < 2:
            v = re.split(r'([(),])', graphFile[i].split(":")[1])
            v = list(filter(lambda x: x not in ['','(',')',','], v))
            l = []
            for i in range(0,len(v),3):
                l.append(Edge(int(v[i].strip("()")), int(v[i+1].strip("()")), int(v[i+2].strip("()"))))
            edges.append(l)
        else:
            v = graphFile[i].split(",")
            if (v[0] != ''):
                cars.append(Car(int(v[0].strip("()")), int(v[1].strip("()"))))
    
    return vertices, edges, cars


def parse_solution(file_name : str, cars : list[Car]):
    solnFile = open(file_name, "r").read().split("\n")
    
    for i in range(len(cars)):
        cars[i].path = list(map(int,solnFile[i].split(":")[1].split(",")))


def validate_solution(cars : list[Car], edges : list[list[Edge]]):
    # Validate the cars are correct and the path actually exists
    for c in cars:
        if (c.src < 4096 and c.dest < 4096 and c.path[0] == c.src and c.path[-1] == c.dest):
            for i in range(1,len(c.path)):
                ok = False
                for edge in edges[c.path[i-1]]:
                    if edge.end == c.path[i]:
                        ok = True
                if not ok:
                    print("Validation Failed! Path is not correct")
                    return False    
        else:
            print("Validation Failed! Graph is not correct")
            return False
    return True

# Simulation
def simulate(vertices : list[Vertex], edges : list[list[Edge]], cars : list[Car]):
    # Initialize the simulation
    locs = []
    cursor = []
    waiting = []
    costs = []
    for i in range(len(cars)):
        locs.append(cars[i].path.pop(0))
        cursor.append(Edge(-1,-1,-1))
        waiting.append(0)
        costs.append(0)

    # Run the simulation
    while locs.count(-1) < len(locs):
        for i in range(len(cars)):
            if waiting[i] == 0:
                if len(cars[i].path) == 0:
                    cursor[i].load -= 1
                    waiting[i] = -1
                    locs[i] = -1
                    cursor[i] = None
                    continue

                next_edge = get_edge(edges[locs[i]], cars[i].path[0])
                if (next_edge.load == next_edge.capacity):
                    waiting[i] = 1
                    costs[i] += 1
                else:
                    cursor[i].load -= 1
                    next_edge.load += 1
                    waiting[i] = get_cost(next_edge, vertices)
                    costs[i] += waiting[i]
                    locs[i] = next_edge.end
                    cursor[i] = next_edge
                    cars[i].path.pop(0)

            
            waiting[i] -= 1

    return costs
            

def main ():
    parser = argparse.ArgumentParser("A solution verification and simulation program")
    parser.add_argument('problem', type=str, help='The input file for the program')
    parser.add_argument('solution', type=str, help='The solution file for the program')
    parser.add_argument('-s', '--simulate', action='store_true', help='Run simulation')

    args = parser.parse_args()
    vertices, edges, cars = parse_problem(args.problem)
    parse_solution(args.solution, cars)
    if not validate_solution(cars, edges):
        print("Validation Failed!")
        return
    
    elif args.simulate:
        costs = simulate(vertices, edges, cars)
        print(f'Total Cost: {sum(costs)}')
        for i in range(len(cars)):
            print(repr(cars[i]) + ': ' + repr(costs[i]))

    

if __name__ == "__main__":
    main()
