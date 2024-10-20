import time
import psutil  # For CPU usage tracking
from memory_profiler import memory_usage
from MatrixMultiplication import MatrixMultiplication

class MatrixMultiplicationTest:
    def __init__(self, matrix_sizes, runs_per_size=5):
        self.matrix_sizes = matrix_sizes
        self.runs_per_size = runs_per_size

    def get_used_memory(self):
        mem = memory_usage(-1, interval=0.1, timeout=1)
        return mem[0]

    def get_cpu_usage(self):
        return psutil.cpu_percent(interval=None)  # Returns CPU usage percentage

    def run_test(self):
        for size in self.matrix_sizes:
            print(f"=== Matrix Size: {size}x{size} ===")
            total_time = 0.0
            total_memory = 0.0
            total_cpu = 0.0  # To store cumulative CPU usage

            for run in range(1, self.runs_per_size + 1):
                mm = MatrixMultiplication(size)
                mm.initialize_matrices()
                memory_before = self.get_used_memory()
                cpu_before = self.get_cpu_usage()
                start_time = time.time()

                mm.multiply()

                end_time = time.time()
                memory_after = self.get_used_memory()
                cpu_after = self.get_cpu_usage()

                elapsed_time = end_time - start_time
                total_time += elapsed_time
                memory_used = memory_after - memory_before
                total_memory += memory_used
                cpu_used = cpu_after - cpu_before  # Difference in CPU usage
                total_cpu += cpu_used

                print(f"Run {run}: Time = {elapsed_time:.3f} seconds, Memory Used = {memory_used:.2f} MB, CPU Used = {cpu_used:.2f}%")

            average_time = total_time / self.runs_per_size
            average_memory = total_memory / self.runs_per_size
            average_cpu = total_cpu / self.runs_per_size

            print(f"Average: Time = {average_time:.3f} seconds, Average Memory Used = {average_memory:.2f} MB, Average CPU Used = {average_cpu:.2f}%\n")

if __name__ == "__main__":
    matrix_sizes = [256, 512, 1024, 2048]
    runs_per_size = 5

    tester = MatrixMultiplicationTest(matrix_sizes, runs_per_size)
    tester.run_test()
