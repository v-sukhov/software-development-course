package ru.krdscp.softdevcourse.javaconcurrency.task04;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class MultithreadSum {
    private IntToIntFunction f;
    private volatile long sum;

    private volatile int m, n, R;

    List<Thread> threadList;

    private int MAX_SEG_LENGTH = 1000;

    public MultithreadSum(IntToIntFunction f){
        this.f = f;
        threadList = Collections.synchronizedList(new ArrayList<Thread>());
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

        this.m = m;
        this.n = n;

        R = m - 1;
        for(int t = 0; t < k; t++) {
            addNewTaskThread();
        }

        for(int i = 0; i < threadList.size(); i++){
            threadList.get(i).join();
        }
        return sum;
    }

    private void addNewTaskThread(){
        int a = R + 1;
        int b = Math.min(a + MAX_SEG_LENGTH - 1, n);
        if (a <= b){
            R = b;
            Task task = new Task(a, b, this);
            Thread th = new Thread(task);
            threadList.add(th);
            th.start();
        }
    }

    synchronized private void addResult(long s){
        sum += s;
        addNewTaskThread();
    }

    public IntToIntFunction getF(){
        return f;
    }
}
