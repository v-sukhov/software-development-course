package ru.krdscp.javaconcurrency.javahelloworld;

public class HelloWorldThread{

    public static void main(String[]args){

        Runnable runnableHello = () -> {
            try{
                Thread.sleep(100);
            }
            catch (InterruptedException e){

            }
            finally {
                System.out.println("Hello");
            }
        };

        Runnable runnableWorld = () -> {
            try{
                Thread.sleep(100);
            }
            catch (InterruptedException e){

            }
            finally {
                System.out.println(" world");
            }
        };

        Thread threadHello = new Thread(runnableHello);
        Thread threadWorld = new Thread(runnableWorld);

        threadHello.start();
        threadWorld.start();

    }
}
