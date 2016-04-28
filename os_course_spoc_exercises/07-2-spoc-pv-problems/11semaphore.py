#coding=utf-8
#semaphore
import threading  
import random  
import time  

class Producer(threading.Thread):
    goods = [1, 2, 3]

    def __init__(self, threadName, semaphore_producer, semaphore_consumer1, semaphore_consumer2, semaphore_consumer3):
        threading.Thread.__init__(self,name=threadName)  
        self.semaphore_producer = semaphore_producer
        self.semaphore_consumer1 = semaphore_consumer1
        self.semaphore_consumer2 = semaphore_consumer2
        self.semaphore_consumer3 = semaphore_consumer3

    def run(self):
        while True:
        	#acquire the semaphore
            self.semaphore_producer.acquire()
            good = random.randrange(1, 4) # 1, 2, 3
            if good == 1:
                print 'Seller is selling: Tape & Battery\n'
                #release the semaphore after execution finishes
                self.semaphore_consumer1.release()
            elif good == 2:
                print 'Seller is selling: Walkman & Battery\n'
                self.semaphore_consumer2.release()
            else:
                print 'Seller is selling: Walkman & Tape\n'
                self.semaphore_consumer3.release()

class Consumer(threading.Thread):
    
    def __init__(self, threadName, need, semaphore_producer, semaphore):
        threading.Thread.__init__(self,name=threadName)  
        self.need = need
        self.semaphore_producer = semaphore_producer
        self.semaphore = semaphore
        
    def run(self):
        while True:
            self.semaphore.acquire()
            if self.need == 1:
                print self.getName()+': Buy Tape & Battery. Listening'
            elif self.need == 2:
                print self.getName()+': Buy Walkman & Battery. Listening'
            else:
                print self.getName()+': Buy Walkman & Tape. Listening'
            time.sleep(2)
            self.semaphore_producer.release()
            

threads=[] #list of threads
#semaphore allows k threads to enter critical section
semaphore_producer = threading.Semaphore(1)
semaphore_consumer1 = threading.Semaphore(0)
semaphore_consumer2 = threading.Semaphore(0)
semaphore_consumer3 = threading.Semaphore(0)
#创建4个threading.Semaphore对象，他最多允许k个线程访问临界区。
#Semaphore类的一个对象用计数器跟踪获取和释放信号量的线程数量。
#create 4 threads
threads.append(Producer("Producer", semaphore_producer, semaphore_consumer1, semaphore_consumer2, semaphore_consumer3))
threads.append(Consumer("Consumer1", 1, semaphore_producer, semaphore_consumer1))
threads.append(Consumer("Consumer2", 2, semaphore_producer, semaphore_consumer2))
threads.append(Consumer("Consumer3", 3, semaphore_producer, semaphore_consumer3))
#创建一个列表，该列表由SemaphoreThread对象构成，start方法开始列表中的每个线程  
#start each thread 
for thread in threads: 
   thread.start() 