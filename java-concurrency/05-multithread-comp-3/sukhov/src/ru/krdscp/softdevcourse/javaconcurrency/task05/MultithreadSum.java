package ru.krdscp.softdevcourse.javaconcurrency.task05;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class MultithreadSum {
    private IntToIntFunction f;
    private volatile long sum;

    private volatile int m, n, R;

    List<Thread> threadList;

    private int MAX_SEG_LENGTH = 10000;

    public MultithreadSum(IntToIntFunction f){
        this.f = f;
        threadList = Collections.synchronizedList(new ArrayList<Thread>());
    }

    private class Segment{
        int a, b;

        public Segment(int a, int b) {
            this.a = a;
            this.b = b;
        }
    }

    private class Task implements Runnable{

        private Segment segment;

        private MultithreadSum parent;

        Task(MultithreadSum parent){
            this.parent = parent;
        }

        @Override
        public void run() {
            while( (segment = parent.getNextSegment()) != null){
                long s = 0;
                for(int t = segment.a; t <= segment.b; t++){
                    s += parent.getF().calc(t);
                }
                parent.addResult(s);
            }
        }
    }

    /**
     * Computes f(m) + ... + f(n) in k threads
     * @param m - segment begin
     * @param n - segment end
     * @param k - threads number
     * @return
     */
    public long calc(int m, int n, int k) throws InterruptedException {
        sum = 0;

        this.m = m;
        this.n = n;

        R = m - 1;
        for(int t = 0; t < k; t++) {
            Task task = new Task(this);
            Thread th = new Thread(task);
            threadList.add(th);
            th.start();
        }

        for(int i = 0; i < threadList.size(); i++){
            threadList.get(i).join();
        }
        return sum;
    }

    private synchronized Segment getNextSegment(){
        int a = R + 1;
        int b = Math.min(a + MAX_SEG_LENGTH - 1, n);
        R = b;
        if (a <= b){
            return new Segment(a, b);
        }
        else{
            return null;
        }
    }

    private synchronized void addResult(long s){
        sum += s;
    }

    public IntToIntFunction getF(){
        return f;
    }
}
