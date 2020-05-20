package com.project.thread; // обязательно

import java.time.Instant;

class AffableThread extends Thread {

    public static boolean is_prime(int number) { // <= O(sqrt(number))
        int i;
        for (i = 2; i * i <= number && number % i != 0; i++) {
        }
        return number % i != 0;
    }

    synchronized public static int get_segment(){ // Здесь все потоки в сумме сделают segments_size действий
        for(; Program.segment_index < Program.segments_size && Program.is_used[Program.segment_index]; Program.segment_index++){
        }
        if(Program.segment_index >= Program.segments_size){
            return -1; // не был найден отрезок
        }
        else{
            Program.is_used[Program.segment_index] = true;
            return Program.segment_index;
        }
    }

    public static int thread_id;

    @Override
    public void run(){	//Этот метод будет выполнен в побочном потоке
        // нужно выдать потоку отрезок
            // посчитать для этого отрезка ответ

        int segment_id = get_segment();
        while(segment_id != -1){
            solve(segment_id);
            segment_id = get_segment();
        }
    }

     public static void solve(int segment_id) {
        for (int i = Program.Begin[segment_id]; i < Program.End[segment_id]; i++) {
            if (is_prime(i)) {
                Program.COUNTS[segment_id]++;
            }
        }
    }
}

public class Program // главный класс
{
    static AffableThread mSecondThread[]; // массив потоков
    public static int COUNTS[]; // массив ответов
    public static int Begin[], End[]; // массив отрезков
    public static boolean is_used[]; // массив использований отрезков

    public static int left, right, number_of_threads, segment_len;

    public static int segments_size, segment_index;

    public static void main(String[] args) throws InterruptedException {
        left = 100;
        right = 1000000;
        number_of_threads = 4;
        segment_len = 30000; // const
        // можно изменять их значения

        // segment_len = 3 * 1e4, simple is_prime
        // 1 : 260-300
        // 2 : 150-170
        // 3 : 130-160
        // 4 : 120-130
        // 5 : 110-130
        // 7 : 100-170
        // 8 : 110-140

        Instant time_begin, time_end; // time
        time_begin = Instant.now();

        mSecondThread = new AffableThread[number_of_threads];// init array
        { // Init Segments

            { // update segments_size
                int len = (int)(right - left + 1), plus = 0;
                if (len % segment_len > 0) { // остался маленький отрезок
                    plus = 1;
                }
                segments_size = len / segment_len + plus;
            }

            //[l, r)
            right++;// [left, right)

            COUNTS = new int[segments_size];
            Begin = new int[segments_size];
            End = new int[segments_size];
            is_used = new boolean[segments_size];
            for(int i = 0; i < segments_size; i++){
                Begin[i] = left + i * segment_len;
                End[i] = Math.min(left + (i + 1) * segment_len, right);
                is_used[i] = false;
                COUNTS[i] = 0;
            }
        }
        segment_index = 0;

        for(int i = 0; i < number_of_threads; i++){ // создание и запуск всех потоков
            mSecondThread[i] = new AffableThread();	//Создание потока
            mSecondThread[i].thread_id = i; // инициализация потока уникальным id
            mSecondThread[i].start();//Запуск потока
        }

        // дождаться выполнения всех потоков
        for(int i = 0; i < number_of_threads; i++) {
            mSecondThread[i].join();
        }

        int count = 0; // считает ответ
        for(int i = 0; i < segments_size; i++){
            count += COUNTS[i];
        }

        time_end = Instant.now(); // output
        System.out.println(count);
        System.out.println((time_end.getNano() - time_begin.getNano()) / 1e6);
    }
}