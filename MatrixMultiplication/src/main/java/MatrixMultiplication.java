import java.util.Random;

public class MatrixMultiplication {
    private int size;
    private double[][] a;
    private double[][] b;
    private double[][] c;

    public MatrixMultiplication(int size) {
        this.size = size;
        a = new double[size][size];
        b = new double[size][size];
        c = new double[size][size];
    }

    public void initializeMatrices() {
        Random random = new Random();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                a[i][j] = random.nextDouble();
                b[i][j] = random.nextDouble();
                c[i][j] = 0.0;
            }
        }
    }

    public void multiply() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }
}

