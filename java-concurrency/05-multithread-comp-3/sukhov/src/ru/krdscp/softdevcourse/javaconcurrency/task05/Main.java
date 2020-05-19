package ru.krdscp.softdevcourse.javaconcurrency.task05;

import java.time.Instant;
import java.time.temporal.ChronoUnit;

public class Main {

    public static void main(String[] args) {

        Instant t1, t2;

        int m = 100;

        int n = 10000000;

        int k = 2;

        //Answer: 664554
        // 1 : 4769
        // 2 : 2494
        // 3 : 1832
        // 4 : 1670
        // 5 : 1527
        // 6 : 1403


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
