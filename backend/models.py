#!/usr/bin/python

class Person:
    def __init__(self, name):
        self.name = name
        self.groups = set()
        self.status = {}
    
    def __hash__(self):
        return hash(self.name)
    
    def join_group(self, group):
        self.group.update(group)
        
    def iocome(self, other, amount):
        if other in self.status:
            self.status[other] += amount
        else:
            self.status[other] = amount
    
    def show_status(self):
        print("==" * 20)
        print("Name: ", self.name)
        print("==" * 20)
        for other in self.status:
            print("\t", other.name, self.status[other])
        print("==" * 20)
        
    
class Transaction:
    def __init__(self, info):
        self.participants = info['participants']
        self.total = info['total']
        self.payer = info['payer']
        self.time = info['time']
        
class Group:
    def __init__(self, members=None):
        if members is not None:
            self.members = set(members)
        else:
            self.members = set()
        self.transactions = []
            
    def add_members(self, new_members):
        self.members.update(set(new_members))
        
    def add_transaction(self, transaction):
        self.transactions.append(transaction)
        total = transaction.total
        num_participants = len(transaction.participants)
        value = total / float(num_participants)
        payer = transaction.payer
        for participant in transaction.participants:
            if participant is not payer:
                participant.iocome(payer, -value)
                payer.iocome(participant, value)
    
    def show_status(self):
        for member in self.members:
            member.show_status()
            
    def simplify(self):
        if len(self.members) < 3:
            return
        
        finished = False
        while not finished:
            finished = True
            triplets = self.__get_triplets()
            for triplet in triplets:
                cnt, i_cnt, o_cnt = 0, [0, 0, 0], [0, 0, 0]
                for i in range(3):
                    a, b = triplet[i], triplet[(i+1) % 3]
                    if a in b.status and b.status[a] != 0:
                        cnt += 1
                        if b.status[a] < 0:
                            i_cnt[i] += 1
                            o_cnt[(i+1) % 3] += 1
                        else:
                            i_cnt[(i+1) % 3] += 1
                            o_cnt[i] += 1
                a, b, c = triplet
                print(a.name, b.name, c.name, cnt, i_cnt, o_cnt)
                if cnt == 2:
                    if max(i_cnt) == 1 and max(o_cnt) == 1:
                        for i in range(3):
                            if i_cnt[i] == o_cnt[i] == 1:
                                b = triplet[i]
                            elif i_cnt[i] == 1 and o_cnt[i] == 0:
                                c = triplet[i]
                            else:
                                a = triplet[i]
                        if b.status[c] == a.status[b]:
                            a.status[c] = b.status[c]
                            c.status[a] = c.status[b]
                            b.status[c] = c.status[b] = a.status[b] = b.status[a] = 0.0
                            finished = False
                        
                elif cnt == 3:
                    if max(i_cnt) == 1 and max(o_cnt) == 1:
                        min_val = min(map(abs, [a.status[b], b.status[c], c.status[a]]))
                        for i in range(3):
                            flag = triplet[i].status[triplet[(i+1)%3]] > 0
                            triplet[i].status[triplet[(i+1)%3]] -= min_val if flag else -min_val 
                            triplet[i].status[triplet[(i+2)%3]] += min_val if flag else -min_val
                        finished = False
                        
                    elif max(i_cnt) == 2 and max(o_cnt) == 2:
                        for i in range(3):
                            if i_cnt[i] == 2 and o_cnt[i] == 0:
                                c = triplet[i]
                            elif i_cnt[i] == 0 and o_cnt[i] == 2:
                                a = triplet[i]
                            else:
                                b = triplet[i]
                        if c.status[b] > b.status[a]:
                            delta = b.status[a]
                            a.status[c] -= delta
                            c.status[a] += delta
                            b.status[a] += delta
                            c.status[b] -= delta
                            b.status[a] = a.status[b] = 0.0
                        else:
                            delta = c.status[b]
                            a.status[c] -= delta
                            c.status[a] += delta
                            a.status[b] += delta
                            b.status[a] -= delta
                            b.status[c] = c.status[b] = 0.0
                        finished = False
            
    def __get_triplets(self):
        members = list(self.members)
        for i, a in enumerate(members):
            for j, b in enumerate(members[i+1:]):
                for c in members[i+j+2:]:
                    yield a, b, c
            
            
