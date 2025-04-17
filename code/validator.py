import re

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


def get_edge(src: list[Edge], dest : int) -> Edge:
    for edge in src:
        if edge.end == dest:
            return edge
    return None

def get_cost(e : Edge):
    start = vertices[e.start]
    end = vertices[e.end]
    return abs(end.x - start.x) + abs(end.y - start.y) 


graphFile = open("valid.test", "r").read().split("\n")


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
        cars.append(Car(int(v[0].strip("()")), int(v[1].strip("()"))))

solnFile = open("valid.soln", "r").read().split("\n")
for i in range(len(solnFile)):
    cars[i].path = list(map(int,solnFile[i].split(":")[1].split(",")))

# Validate the cars are correct and the path actually exists
for c in cars:
    if (c.src < 4096 and c.dest < 4096 and c.path[0] == c.src and c.path[-1] == c.dest):
        for i in range(1,len(c.path)):
            ok = False
            for edge in edges[c.path[i-1]]:
                if edge.end == c.path[i]:
                    ok = True
            if not ok:
                print("Validation Failed!")
                break    
    else:
        print("Validation Failed!")

# Simulation

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

time = 0
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
            else:
                cursor[i].load -= 1
                next_edge.load += 1
                waiting[i] = get_cost(next_edge)
                costs[i] += waiting[i]
                locs[i] = next_edge.end
                cursor[i] = next_edge
                cars[i].path.pop(0)

        
        waiting[i] -= 1

print(costs)
            

            