import oshi.SystemInfo;
import oshi.hardware.CentralProcessor;
import java.io.FileWriter;
import java.io.IOException;

public class MatrixMultiplicationTest {

    public static void main(String[] args) {
        int[] matrixSizes = {256, 512, 1024, 2048};
        int runsPerSize = 5;

        // Initialize OSHI for CPU usage tracking
        SystemInfo si = new SystemInfo();
        CentralProcessor processor = si.getHardware().getProcessor();

        // Store the initial CPU ticks
        long[] previousTicks = processor.getSystemCpuLoadTicks();

        try (FileWriter writer = new FileWriter("benchmark/results.csv")) {
            writer.write("Matrix Size,Run,Time (seconds),Memory Used (MB),CPU Used (%)\n");

            for (int size : matrixSizes) {
                System.out.println("=== Matrix Size: " + size + "x" + size + " ===");
                double totalTime = 0.0;
                long totalMemory = 0;
                double totalCpu = 0.0;

                for (int run = 1; run <= runsPerSize; run++) {
                    MatrixMultiplication mm = new MatrixMultiplication(size);
                    mm.initializeMatrices();
                    long memoryBefore = getUsedMemory();

                    long startTime = System.nanoTime();
                    mm.multiply();
                    long endTime = System.nanoTime();

                    long memoryAfter = getUsedMemory();

                    // Get the CPU usage based on the previous ticks
                    double cpuUsed = processor.getSystemCpuLoadBetweenTicks(previousTicks) * 100;  // Returns CPU usage as a percentage
                    previousTicks = processor.getSystemCpuLoadTicks();  // Update previous ticks

                    // Calculate time and memory
                    double elapsedTime = (endTime - startTime) / 1e9;  // Convert to seconds
                    long memoryUsed = (memoryAfter - memoryBefore) / (1024 * 1024);  // Convert to MB

                    // Add to totals
                    totalTime += elapsedTime;
                    totalMemory += memoryUsed;
                    totalCpu += cpuUsed;

                    // Print and write results to file
                    System.out.printf("Run %d: Time = %.3f seconds, Memory Used = %d MB, CPU Used = %.2f%%%n", run, elapsedTime, memoryUsed, cpuUsed);
                    writer.write(String.format("%d,%d,%.3f,%d,%.2f\n", size, run, elapsedTime, memoryUsed, cpuUsed));
                }

                // Average results
                double averageTime = totalTime / runsPerSize;
                double averageMemory = (double) totalMemory / runsPerSize;
                double averageCpu = totalCpu / runsPerSize;
                System.out.printf("Average: Time = %.3f seconds, Average Memory Used = %.2f MB, Average CPU Used = %.2f%%%n%n", averageTime, averageMemory, averageCpu);
            }
        } catch (IOException e) {
            System.err.println("Error writing to file: " + e.getMessage());
        }
    }

    // Method to get used memory in bytes
    private static long getUsedMemory() {
        Runtime runtime = Runtime.getRuntime();
        return runtime.totalMemory() - runtime.freeMemory();
    }
}
