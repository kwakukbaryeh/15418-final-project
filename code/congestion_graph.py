import argparse, re, tkinter as tk, colorsys, time

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

def get_gradient_color(load, capacity):
    norm = load / capacity
    h = (1/3) * norm
    r,g,b = colorsys.hsv_to_rgb(h, 1, 1)
    r = int(r * 255)
    g = int(g * 255)
    b = int(b * 255)
    return "#%02x%02x%02x" % (r, g, b)

# Tkinter GUI Globals
root = None
canvas = None
scale = 15
r = 5 # Vertex radius
s = 128 * scale + r

def setup_gui():
    global root, canvas, scale, r, s

    root = tk.Tk()
    root.title("Congestion Graph")
    canvas = tk.Canvas(root, width=s, height=s, bg="black")
    canvas.pack()

def draw_frame(vertices, edges):
    global canvas, scale, r, s

    canvas.delete("sim")

    for el in edges:
        for edge in el:
            x1 = vertices[edge.start].x*scale
            y1 = vertices[edge.start].y*scale
            x2 = vertices[edge.end].x*scale
            y2 = vertices[edge.end].y*scale
            # color = "#%02x%02x%02x" % (55, 55, 55)
            if edge.load == 0:
                continue
            color = get_gradient_color(edge.load, edge.capacity)
            canvas.create_line(x1, y1, x2, y1, tags='sim', fill=color)
            canvas.create_line(x2, y1, x2, y2, tags='sim', fill=color)
        
    for i in range(len(vertices)):
        x = vertices[i].x*scale
        y = vertices[i].y*scale
        canvas.create_oval(x-r, y-r, x+r, y+r, tags='sim', fill="gray")

    canvas.update_idletasks()

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

    # Setup GUI
    setup_gui()

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
                else:
                    cursor[i].load -= 1
                    next_edge.load += 1
                    waiting[i] = get_cost(next_edge, vertices)
                    locs[i] = next_edge.end
                    cursor[i] = next_edge
                    cars[i].path.pop(0)

            
            waiting[i] -= 1
        
        draw_frame(vertices, edges)


def main():
    parser = argparse.ArgumentParser("A solution verification and simulation program")
    parser.add_argument('problem', type=str, help='The input file for the program')
    parser.add_argument('solution', type=str, help='The solution file for the program')

    args = parser.parse_args()
    vertices, edges, cars = parse_problem(args.problem)
    parse_solution(args.solution, cars)
    simulate(vertices, edges, cars)


if __name__ == "__main__":
    main()