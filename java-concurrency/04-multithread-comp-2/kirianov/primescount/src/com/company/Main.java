package com.company;

import java.util.Scanner;
import java.util.concurrent.TimeUnit;

public class Main {
    static int prime[] = new int[10000000];
    static boolean isprime[] = new boolean[10000000];
    static int nextpos = 1;
    public static class Calc implements Runnable {
        int l, r, ld, rd;
        public Calc(int _l, int _r, int _ld, int _rd){
            l = _l;
            r = _r;
            ld = _ld;
            rd = _rd;
        }

        public void run() {
            for(int i = l; i<r; i++){
                if(!isprime[i])
                    continue;
                for(int j = ld; j<rd; j++){
                    if(i%prime[j]==0){
                        isprime[i] = false;
                        break;
                    }
                }
            }
        }
    }
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        System.out.println("Input l and r");
        int l = in.nextInt(), r = in.nextInt();
        System.out.println("Input number of threads");
        int k = in.nextInt();
        for(int i = 0; i<r; i++){
            isprime[i] = true;
        }
        isprime[0] = false;
        isprime[1] = false;
        prime[0] = 2;
        int ming = 0, maxg = 1, lef = 3;
        //calculating
        while(r-lef>5){
            int temp = ((r-lef)/k)+1;
            for(int i = 0; i<k; i++){
                Thread thr = new Thread(new Calc(lef+i*temp,lef+(i+1)*temp, ming, maxg));
                thr.run();
            }
            for(int i = lef; i<lef+temp; i++){
                if(isprime[i]){
                    prime[nextpos] = i;
                    nextpos++;
                }
            }
            lef+=temp;
            ming = maxg;
            maxg = nextpos;
        }
        //last 5 numbers are counted here
        for(int i = lef; i<r; i++){
            if(!isprime[i])
                continue;
            for(int j = ming; j<maxg; j++){
                if(i%prime[j]==0){
                    isprime[i] = false;
                    break;
                }
            }
            for(int j = lef; j<i; j++){
                if(i%j==0){
                    isprime[i] = false;
                    break;
                }
            }
            if(isprime[i]){
                prime[nextpos] = i;
                nextpos++;
            }
        }
        //counting from l to r
        int ans = 0;
        for(int i = 0; i<nextpos; i++){
            if(prime[i]>=l && prime[i]<r)
                ans++;
        }
        System.out.print("There are ");
        System.out.print(ans);
        System.out.print(" prime numbers between ");
        System.out.print(l);
        System.out.print(" and ");
        System.out.print(r);
    }
}
