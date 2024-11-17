import java.util.Comparator;

class Compare implements Comparator<Individual> {
    public int compare(Individual lhs, Individual rhs) {
        return lhs.compareTo(rhs);
    }
}