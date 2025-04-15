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
        self.cost = dict()
    
    def __repr__(self):
        return f'({self.start},{self.end},{self.capacity}, {self.load}, {self.cost})'

class Car:
    def __init__(self, src : int, dest : int):
        self.src = src
        self.dest = dest
        self.path = []

graphFile = open("inputs/easy_4096.test", "r").read().split("\n")


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

solnFile = open("easy_soln.txt", "r").read().split("\n")
for i in range(len(solnFile)):
    cars[i].path = list(map(int,solnFile[i].split(":")[1].split(",")))

# Validate the cars are correct and the path actually exists
for c in cars:
    if (c.src < 4096 and c.dest < 4096 and c.path[0] == c.src and c.path[-1] == c.dest):
        for i in range(1,len(path)):
            path[i-1]
