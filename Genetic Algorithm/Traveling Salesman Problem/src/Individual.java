import java.util.ArrayList;
import java.util.List;


class Individual implements Comparable<Individual> {
    List<Integer> genes = new ArrayList<>();
    double result = 0;

    public void findResult(int[] x, int[] y) {
        result = 0;
        for (int i = 0; i < genes.size() - 1; i++) {
            int a = genes.get(i);
            int b = genes.get(i + 1);
            result += Main.distance(x[a], y[a], x[b], y[b]);
        }
    }
    public void findResult(double[] x, double[] y) {
        result = 0;
        for (int i = 0; i < genes.size() - 1; i++) {
            int a = genes.get(i);
            int b = genes.get(i + 1);
            result += Main.distance(x[a], y[a], x[b], y[b]);
        }
    }

    @Override
    public int compareTo(Individual other) {
        return Double.compare(this.result, other.result);
    }
}
