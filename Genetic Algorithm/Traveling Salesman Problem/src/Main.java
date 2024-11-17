import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Random;
import java.util.Scanner;
import java.util.Set;

public class Main {
        static int[] x;
        static int[] y;
        static int n;
        static PriorityQueue<Individual> q = new PriorityQueue<>(new Compare());
        static PriorityQueue<Individual> nextQ = new PriorityQueue<>(new Compare());
        static Random rand = new Random();

        public static double distance(int x1, int y1, int x2, int y2) {
            return Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        }

        public static Individual findBest() {
            return q.peek();
        }

        public static void printIndividual(Individual individual) {
            System.out.println("Current best result: " + individual.result);
        }

        public static void printPath(Individual individual) {
            System.out.print("Path: ");
            for (int gene : individual.genes) {
                System.out.print(gene + " ");
            }
            System.out.println("\nCurrent best result: " + individual.result);
        }

        public static void mutate(Individual i) {
            int randGene1 = rand.nextInt(n);
            int randGene2 = rand.nextInt(n);
            Collections.swap(i.genes, randGene1, randGene2);
        }

        public static void fillRestOfChildWithParent(Individual child, Individual parent, int stopper) {
            Set<Integer> childGenesSet = new HashSet<>(child.genes);
            for (int i = 0; i < n; i++) {
                if (!childGenesSet.contains(parent.genes.get(i))) {
                    child.genes.add(parent.genes.get(i));
                }
            }
        }

        public static void cross(Individual p1, Individual p2) {
            Individual c1 = new Individual();
            Individual c2 = new Individual();

            int stopper = rand.nextInt(n);
            c1.genes.addAll(p1.genes.subList(0, stopper + 1));
            c2.genes.addAll(p2.genes.subList(0, stopper + 1));

            fillRestOfChildWithParent(c1, p2, stopper);
            fillRestOfChildWithParent(c2, p1, stopper);

            mutate(c1);
            mutate(c2);

            c1.findResult(x, y);
            c2.findResult(x, y);

            nextQ.add(c1);
            nextQ.add(c2);
        }

        public static void reproduce() {
            int initSize = q.size();
            while (q.size() > initSize / 2) {
                Individual i1 = q.poll();
                Individual i2 = q.poll();
                cross(i1, i2);

                nextQ.add(i1);
                nextQ.add(i2);
            }
            q.clear();
        }

        public static void generateRandomCoordinates() {
            for (int i = 0; i < n; i++) {
                x[i] = rand.nextInt(10);
                y[i] = rand.nextInt(10);
            }
        }

        public static void initNextGen() {
            q = new PriorityQueue<>(nextQ);
            nextQ.clear();
        }

        public static void main(String[] args) {
            Scanner sc = new Scanner(System.in);
            n = sc.nextInt();

            x = new int[n];
            y = new int[n];

            generateRandomCoordinates();

            List<Individual> parents = new ArrayList<>();
            int numberPop = 10;

            for (int i = 0; i < numberPop; i++) {
                Individual individ = new Individual();
                for (int j = 0; j < n; j++) {
                    individ.genes.add(j);
                }
                Collections.shuffle(individ.genes);
                individ.findResult(x, y);
                parents.add(individ);
                q.add(individ);
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