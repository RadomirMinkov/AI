import java.util.*;

public class Main {
    static int[] x;
    static int[] y;
    static int n;
    static double[] hardCodeX;
    static double[] hardCodeY;
    static String[] cities;
    static PriorityQueue<Individual> queue = new PriorityQueue<>(new Compare());
    static PriorityQueue<Individual> nextQueue = new PriorityQueue<>(new Compare());
    static Random rand = new Random(42);

    public static double distance(int x1, int y1, int x2, int y2) {
        return Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }

    public static double distance(double x1, double y1, double x2, double y2) {
        return Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }


    public static Individual findBest() {
        return queue.peek();
    }

    public static void printIndividual(Individual individual) {
        System.out.println("Current best result: " + individual.result);
    }

    public static void printPath(Individual individual) {
        System.out.print("Path: ");
        for (int gene : individual.genes) {
            System.out.print(cities[gene] + " ");
        }
        System.out.println("\nCurrent best result: " + individual.result);
    }

    public static void mutate(Individual i) {
        int randGene1 = rand.nextInt(n);
        int randGene2 = rand.nextInt(n);
        Collections.swap(i.genes, randGene1, randGene2);
    }

    public static void fillRestOfChildWithParent(Individual child, Individual parent, Set<Integer> usedGenes) {
        for (int gene : parent.genes) {
            if (!usedGenes.contains(gene)) {
                child.genes.add(gene);
                usedGenes.add(gene);
            }
        }
    }

    public static void cross(Individual p1, Individual p2) {
        Individual c1 = new Individual();
        Individual c2 = new Individual();

        int point1 = rand.nextInt(n);
        int point2 = rand.nextInt(n);
        if (point1 > point2) {
            int temp = point1;
            point1 = point2;
            point2 = temp;
        }

        Set<Integer> usedGenesC1 = new HashSet<>();
        Set<Integer> usedGenesC2 = new HashSet<>();

        // Add genes between the two points
        for (int i = point1; i <= point2; i++) {
            c1.genes.add(p1.genes.get(i));
            c2.genes.add(p2.genes.get(i));
            usedGenesC1.add(p1.genes.get(i));
            usedGenesC2.add(p2.genes.get(i));
        }

        fillRestOfChildWithParent(c1, p2, usedGenesC1);
        fillRestOfChildWithParent(c2, p1, usedGenesC2);

        mutate(c1);
        mutate(c2);

        c1.findResult(hardCodeX, hardCodeY);
        c2.findResult(hardCodeX, hardCodeY);

        nextQueue.add(c1);
        nextQueue.add(c2);
    }

    public static void reproduce() {
        int initSize = queue.size();
        while (queue.size() > initSize / 2) {
            Individual i1 = queue.poll();
            Individual i2 = queue.poll();
            cross(i1, i2);

            nextQueue.add(i1);
            nextQueue.add(i2);
        }
        queue.clear();
    }

    public static void initNextGen() {
        queue = new PriorityQueue<>(nextQueue);
        nextQueue.clear();
    }

    public static void generateRandomCoordinates() {
        for (int i = 0; i < n; i++) {
            x[i] = rand.nextInt(10);
            y[i] = rand.nextInt(10);
        }
    }


    public static void initCityData() {
        cities = new String[]{"Aberystwyth", "Brighton", "Edinburgh", "Exeter", "Glasgow",
                "Inverness", "Liverpool", "London", "Newcastle", "Nottingham",
                "Oxford", "Stratford"};

        hardCodeX = new double[]{0.000190032, 383.458, -27.0206, 335.751, 69.4331,
                168.521, 320.350, 179.933, 492.671, 112.198,
                306.320, 217.343};

        hardCodeY = new double[]{-0.000285946, -0.000608756, -282.758, -269.577, -246.780,
                31.4012, -160.900, -318.031, -131.563, -110.561,
                -108.090, -447.089};

        n = cities.length;
    }

    public static void main(String[] args) {
        /*
        Scanner sc = new Scanner(System.in);
        n = sc.nextInt();
        x = new int[n];
        y = new int[n];

        generateRandomCoordinates();

         */
        initCityData();
        List<Individual> parents = new ArrayList<>();
        int numberPop = 10;

        for (int i = 0; i < numberPop; i++) {
            Individual individ = new Individual();
            for (int j = 0; j < n; j++) {
                individ.genes.add(j);
            }
            Collections.shuffle(individ.genes);
            individ.findResult(hardCodeX, hardCodeY);
            parents.add(individ);
            queue.add(individ);
        }

        int iter = 0;
        Individual best = null;

        while (iter <= 1000) {
            if (iter % 100 == 0) {
                System.out.print(iter + ": ");
                best = findBest();
                printIndividual(best);
            }
            if (iter == 1000) {
                printPath(best);
            }

            reproduce();
            initNextGen();
            iter++;
        }
    }
}