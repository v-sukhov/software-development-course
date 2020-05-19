package ru.krdscp.softdevcourse.javaconcurrency.task04;

import java.time.Instant;
import java.time.temporal.ChronoUnit;

public class Main {

    public static void main(String[] args) {

        Instant t1, t2;

        int m = 100;

        int n = 10000000;

        int k = 6;

        //Answer: 664554
        // 1 : 7900
        // 2 : 3991
        // 3 : 2708
        // 4 : 2263
        // 5 : 2088
        // 6 : 1963

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
