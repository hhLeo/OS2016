#! /usr/bin/env python
#输入格式参考page-replacement-option.py,置换算法是PFF(不命中率算法)
#输入选项中option 只能输入“PFF”，否则出错
#

import sys
from optparse import OptionParser
import random
import math

def hfunc(index):
    if index == -1:
        return 'MISS'
    else:
        return 'HIT '

def vfunc(victim):
    if victim == -1:
        return '-'
    else:
        return str(victim)

#
# main program
#
parser = OptionParser()
parser.add_option('-a', '--addresses', default='-1',   help='a set of comma-separated pages to access; -1 means randomly generate',  action='store', type='string', dest='addresses')
parser.add_option('-p', '--policy', default='FIFO',    help='replacement policy: FIFO, LRU, OPT, CLOCK',                action='store', type='string', dest='policy')
parser.add_option('-b', '--clockbits', default=1,      help='for CLOCK policy, how many clock bits to use',                          action='store', type='int', dest='clockbits')
parser.add_option('-f', '--pageframesize', default='3',    help='size of the physical page frame, in pages',                                  action='store', type='string', dest='pageframesize')
parser.add_option('-s', '--seed', default='0',         help='random number seed',                                                    action='store', type='string', dest='seed')
parser.add_option('-N', '--notrace', default=False,    help='do not print out a detailed trace',                                     action='store_true', dest='notrace')
parser.add_option('-c', '--compute', default=False,    help='compute answers for me',                                                action='store_true', dest='solve')

(options, args) = parser.parse_args()

print 'ARG addresses', options.addresses
print 'ARG policy', options.policy
print 'ARG clockbits', options.clockbits
print 'ARG pageframesize', options.pageframesize
print 'ARG seed', options.seed
print 'ARG notrace', options.notrace
print ''

addresses   = str(options.addresses)
pageframesize   = int(options.pageframesize)
seed        = int(options.seed)
policy      = str(options.policy)
notrace     = options.notrace
clockbits   = int(options.clockbits)

random.seed(seed)

addrList = []
addrList = addresses.split(',')

if options.solve == False:
    print 'Assuming a replacement policy of %s, and a physical page frame of size %d pages,' % (policy, pageframesize)
    print 'figure out whether each of the following page references hit or miss'

    for n in addrList:
        print 'Access: %d  Hit/Miss?  State of Memory?' % int(n)
    print ''

else:
    if notrace == False:
        print 'Solving...\n'

    # init memory structure
    count = 0 ## the length of memory
    memory = []
    hits = 0
    miss = 0
    T = 2; ## init the window_size
    lastT = -1; ##init the last time of miss, the time of miss is the same as index of addresslist

    if policy == 'PFF':
        leftStr = 'PFF'
        riteStr = ' .. '
    #     leftStr = 'FirstIn'
    #     riteStr = 'Lastin '
    # elif policy == 'LRU':
    #     leftStr = 'LRU'
    #     riteStr = 'MRU'
    # elif policy == 'OPT' or  policy == 'CLOCK':
    #     leftStr = 'Left '
    #     riteStr = 'Right'
    else:
        print 'Policy %s is not yet implemented' % policy
        exit(1)
    
    # track reference bits for clock
    #ref   = {}

    cdebug = False

    # need to generate addresses
    addrIndex = 0
    for nStr in addrList:
        # first, lookup
        n = int(nStr)
        try:
            idx = memory.index(n) #if not wrong hits++, the same as hit memory wrong
            hits = hits + 1  #if hit
            # if policy == 'LRU' :  
            #     update = memory.remove(n) ## n is the newest
            #     memory.append(n) # puts it on MRU side
        except:
            idx = -1 # miss
            miss = miss + 1

        victim = [] #init the page to be changed, the pages to be replaced may be just one         
        if idx == -1: #miss , need to replace?
            # miss, replace?
            # print 'BUG count, pageframesize:', count, pageframesize
            print 'replace? -> addrIndex : %d   lastT : %d\n' % (addrIndex, lastT)
            if addrIndex - lastT > T: # Time of current miss - Time of last miss > T , remove the pages not used while lastT to current
                # must replace
                print 'must replace -> addrIndex : %d   lastT : %d\n' % (addrIndex, lastT) 
                for mem in memory:
                    toremove = True;
                    for i in range(lastT, addrIndex):
                        if(mem == int(addrList[i])): ## used while lastT to addrIndex
                            toremove = False;
                    if(toremove):
                        print 'to remove %d in memory \n' % (mem)
                        memory.remove(mem)
                        victim.append(mem)
            else:
                # miss, but no replacement needed (current - lastT <= T)
                victim = -1
                count = count + 1
             
            lastT = addrIndex;
            # now add to memory
            memory.append(n) 
            if cdebug:
                print 'LEN (a)', len(memory)
            if victim != -1:
                assert(victim not in memory)

        if notrace == False:
            print 'Access: %d  %s %s -> %12s <- %s Replaced:%4s [Hits:%d Misses:%d]' % (n, hfunc(idx), leftStr, memory, riteStr, vfunc(victim), hits, miss)
        addrIndex = addrIndex + 1
        
    print ''
    print 'FINALSTATS hits %d   misses %d   hitrate %.2f' % (hits, miss, (100.0*float(hits))/(float(hits)+float(miss)))
    print ''

    
    
    







