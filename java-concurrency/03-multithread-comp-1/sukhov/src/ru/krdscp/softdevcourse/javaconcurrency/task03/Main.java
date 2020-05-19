package ru.krdscp.softdevcourse.javaconcurrency.task03;

import java.time.Instant;
import java.time.temporal.ChronoUnit;

public class Main {

    public static void main(String[] args) {

        Instant t1, t2;

        int m = 100;

        int n = 10000000;

        int k = 5;

        //Answer: 664554
        // 1 : 4725
        // 2 : 2987
        // 3 : 2265
        // 4 : 1953
        // 5 : 1708
        // 6 : 1562
        // 7 : 1476
        // 8 : 1411
        // 9 : 1332
        // 10 : 1310
        // 11 : 1344
        // 12 : 1305


        IsPrimeNumber f = new IsPrimeNumber();

        MultithreadSum mSum = new MultithreadSum(f);

        try {
            t1 = Instant.now();
            System.out.println(mSum.calc(m, n, k));
            t2 = Instant.now();
            System.out.println("Time: " + ChronoUnit.MILLIS.between(t1,t2));
        } catch (InterruptedException e) {
            System.out.println("Something went wrong...");
            e.printStackTrace();
        }
    }
}
