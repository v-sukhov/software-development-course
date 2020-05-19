package ru.krdscp.softdevcourse.javaconcurrency.task03;

public class IsPrimeNumber implements IntToIntFunction{
    @Override
    public int calc(int n) {

        int i;

        for(i = 2; i * i <= n && n % i != 0; i++){
        }

        return n % i == 0 ? 0 : 1;
    }
}
