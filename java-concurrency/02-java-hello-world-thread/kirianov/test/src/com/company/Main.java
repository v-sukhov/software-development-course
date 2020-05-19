package com.company;

import java.util.concurrent.TimeUnit;

public class Main {
    public static class MyRunnable implements Runnable {
        String s;
        public MyRunnable(String s1) {
            s = s1;
        }

        public void run() {
            System.out.print(s);
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    public static void main(String[] args) {
        Thread thread1 = new Thread(new MyRunnable("Hello "));
        Thread thread2 = new Thread(new MyRunnable("World"));
        thread1.run();

        thread2.run();
    }
}
