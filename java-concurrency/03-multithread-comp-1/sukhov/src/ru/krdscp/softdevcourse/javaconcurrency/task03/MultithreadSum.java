package ru.krdscp.softdevcourse.javaconcurrency.task03;

import java.util.ArrayList;

public class MultithreadSum {
    private IntToIntFunction f;
    private volatile long sum;

    ArrayList<Thread> threadList;

    public MultithreadSum(IntToIntFunction f){
        this.f = f;
        threadList = new ArrayList<>();
    }

    private class Task implements Runnable{

        private int a,b;

        private MultithreadSum parent;

        Task(int a, int b, MultithreadSum parent){
            this.a = a;
            this.b = b;
            this.parent = parent;
        }

        @Override
        public void run() {
            long s = 0;
            for(int t = a; t <= b; t++){
                s += parent.getF().calc(t);
            }
            parent.addResult(s);
        }
    }

    /**
     * Computes f(m) + ... + f(n) in k threads
     * @param m - segment begin
     * @param n - segment end
     * @param k - threads number
     * @return
     */
    long calc(int m, int n, int k) throws InterruptedException {
        sum = 0;

        for(int t = 0; t < k; t++){
            int a = (int)(m + (long)(n + 1 - m) * t / k);
            int b = (int)(m + (long)(n + 1 - m) * (t + 1) / k);
            Task task = new Task(a, b-1, this);
            Thread th = new Thread(task);
            th.start();
            threadList.add(th);
        }

        for(int i = 0; i < threadList.size(); i++){
            threadList.get(i).join();
        }
        return sum;
    }

    private synchronized void addResult(long s){
        sum += s;
    }

    public IntToIntFunction getF(){
        return f;
    }
}
