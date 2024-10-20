import random

class MatrixMultiplication:
    def __init__(self, size):
        self.size = size
        self.a = [[0.0 for _ in range(size)] for _ in range(size)]
        self.b = [[0.0 for _ in range(size)] for _ in range(size)]
        self.c = [[0.0 for _ in range(size)] for _ in range(size)]

    def initialize_matrices(self):
        for i in range(self.size):
            for j in range(self.size):
                self.a[i][j] = random.random()
                self.b[i][j] = random.random()
                self.c[i][j] = 0.0

    def multiply(self):
        for i in range(self.size):
            for j in range(self.size):
                for k in range(self.size):
                    self.c[i][j] += self.a[i][k] * self.b[k][j]
