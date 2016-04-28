#coding=utf-8
#condition
import threading  
import random  
import time  

condition = threading.Condition()
product = 0
class Producer(threading.Thread):
    goods = [1, 2, 3]

    def __init__(self, threadName):
        threading.Thread.__init__(self,name=threadName)  
        
    def run(self):
        global condition, product
        while True:
            if condition.acquire():
                if product == 0:
                    product = random.randrange(1, 4)
                    if product == 1:
                        print 'Seller is selling: Tape & Battery'
                    elif product == 2:
                        print 'Seller is selling: Walkman & Battery'
                    else:
                        print 'Seller is selling: Walkman & Tape'
                    condition.notifyAll()
                else:
                    condition.wait()
                condition.release()
                
class Consumer(threading.Thread):
    
    def __init__(self, threadName, need):
        threading.Thread.__init__(self,name=threadName)  
        self.need = need
               
    def run(self):
        global condition, product
        while True:
            if condition.acquire():
                if product == self.need:
                    if self.need == 1:
                        print self.getName()+': Buy Tape & Battery.\n'
                    elif self.need == 2:
                        print self.getName()+': Buy Walkman & Battery.\n'
                    else:
                        print self.getName()+': Buy Walkman & Tape.\n'
                    product = 0
                    time.sleep(2)
                    condition.notifyAll()
                else:
                    condition.wait()
                condition.release()

threads=[]
semaphore_producer = threading.Semaphore(1)
semaphore_consumer1 = threading.Semaphore(0)
semaphore_consumer2 = threading.Semaphore(0)
semaphore_consumer3 = threading.Semaphore(0)

threads.append(Producer("Producer"))
threads.append(Consumer("Consumer1", 1))
threads.append(Consumer("Consumer2", 2))
threads.append(Consumer("Consumer3", 3))

for thread in threads: 
   thread.start() 