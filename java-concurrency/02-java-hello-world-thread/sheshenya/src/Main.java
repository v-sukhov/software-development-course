package src;

import java.util.concurrent.TimeUnit;

public class Main {
    public static void main(String[] args) {
        
        Runnable PrintHello = () -> {
            System.out.print("Hello, ");
        };
        Runnable PrintWorld = () -> {
            System.out.println("world!");
        };

        Thread thread1 = new Thread(PrintHello);
        Thread thread2 = new Thread(PrintWorld);

        thread1.start();

        try {
            TimeUnit.MILLISECONDS.sleep(500);
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        thread2.start();
    }
}