import java.util.ArrayList;
import java.util.List;

import static java.awt.geom.Point2D.distance;

class Individual implements Comparable<Individual> {
    List<Integer> genes = new ArrayList<>();
    double result = 0;

    public void findResult(int[] x, int[] y) {
        result = 0;  // Reset result to ensure it's recalculated properly
        for (int i = 0; i < genes.size() - 1; i++) {
            int a = genes.get(i);
            int b = genes.get(i + 1);
            result += distance(x[a], y[a], x[b], y[b]);
        }
    }

    @Override
    public int compareTo(Individual other) {
        return Double.compare(this.result, other.result);  // Min-Heap priority queue
    }
}
