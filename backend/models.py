#!/usr/bin/python
import os
import json
import pickle

class Person:
    def __init__(self, name):
        self.name = name
        self.groups = set()
        self.status = {}
    
    def join_group(self, group):
        self.groups.update([group,] )
        
    def iocome(self, other, amount):
        if other in self.status:
            self.status[other] += amount
        else:
            self.status[other] = amount
    
    def query_status(self):
        status = { 
                "name": self.name,
                }
        status['relation'] = [
                (other.name, self.status[other]) 
                for other in self.status
                if self.status[other] != 0.0
                ]
        status['groups'] = [g.name for g in list(self.groups)]
        return status

    def show_status(self):
        print("==" * 20)
        print("Name: ", self.name)
        print("==" * 20)
        for other in self.status:
            if self.status[other] != 0.0:
                print("\t", other.name, self.status[other])
        print("==" * 20)
        
    
class Transaction:
    def __init__(self, info):
        self.participants = info['participants']
        self.total = info['total']
        self.payer = info['payer']
        self.time = info['time']
        
    def __str__(self):
        participants = ', '.join([person.name for person in self.participants])
        payer = self.payer.name
        return 'Time: %s\nParticipants: %s\nPayer: %s\nTotal %s' % (self.time, participants, payer, self.total)
    
    __repr__ = __str__
    
class Group:
    def __init__(self, name='default_name', members=None):
        if members is not None:
            self.members = set(members)
        else:
            self.members = set()
        self.transactions = []
        self.name = name
        
    def add_members(self, new_members):
        self.members.update(set(new_members))
        
    def add_transaction(self, transaction):
        self.transactions.append(transaction)
        total = transaction.total
        num_participants = len(transaction.participants)
        value = total / float(num_participants)
        payer = transaction.payer
        for member in self.members:
            if payer not in self.members:
                return False
        for participant in transaction.participants:
            if participant not in self.members:
                return False
        for participant in transaction.participants:
            if participant is not payer:
                participant.iocome(payer, -value)
                payer.iocome(participant, value)
        return True
                
    def add_transactions(self, transactions):
        for transaction in transactions:
            self.add_transaction(transaction)
   
    def query_member_status(self):
        if not self.members: return "No Members"
        return [member.query_status() for member in self.members]

    def show_status(self):
        for member in self.members:
            member.show_status()
    
    def show_transaction(self):
        if not self.transactions: return "No Transactions"
        return 'Transactions:\n' + '\n'.join(map(str, self.transactions))
    
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
            
class Management:
    
    def __init__(self, save_path='.'):
        self.people = {}
        self.groups = {}
        self.save_path = save_path
        self.FILENAME_PEOPLE = 'people'
        self.FILENAME_GROUP = 'group'
        
    def __add_person(self, person):
        if person.name in self.people:
            return
        self.people[person.name] = person
    
    def __add_group(self, group):
        if group.name in self.groups:
            return
        self.groups[group.name] = group
    
    def add_person(self, name):
        if name not in self.people:
            new_person = Person(name)
            self.people[name] = new_person
            return True
        else:
            return False
    
    def add_group(self, name):
        if name not in self.groups:
            new_group = Group(name)
            self.groups[name] = new_group
            return True
        else:
            return False
        
    def people_join_group(self, people_names, group_name):
        try:
            group = self.groups[group_name]
            for person_name in people_names:
                if person_name in self.people:
                    person = self.people[person_name]
                    person.join_group(group)
                    group.add_members([person, ])
                    
            return True
        except Exception as e:
            return False
    
    def add_transaction(self, group_name, transaction_info):
        try:
            if group_name not in self.groups:
                return False
            participants = [self.people[person_name] for person_name in transaction_info['participants']]
            payer = self.people[transaction_info['payer']]
            transaction_info['participants'] = participants
            transaction_info['payer'] = payer
            return self.groups[group_name].add_transaction(Transaction(transaction_info))
        except Exception as e:
            print e 
            return False
    
    def query_group_status(self, group_name):
        if group_name not in self.groups:
            return
        group = self.groups[group_name]
        
        group_info = {
                'group_name': group_name,
                'group_members': [m.name for m in list(group.members)],
                'group_size': len(group.members),
                'group_member_status': group.query_member_status()
                }
        return group_info
   
    def query_user_status(self, user_name):
        if user_name not in self.people:
            return

        user = self.people[user_name]
        return user.query_status()
    
    def save_status(self):
        with open(os.path.join(self.save_path, self.FILENAME_PEOPLE), 'w') as f_people:
            pickle.dump(self.people, f_people)
        
        with open(os.path.join(self.save_path, self.FILENAME_GROUP), 'w') as f_group:
            pickle.dump(self.groups, f_group)
    
    
    def load_status(self, load_path):
        with open(os.path.join(self.save_path, self.FILENAME_PEOPLE), 'r') as f_people:
            self.people = pickle.load(f_people)
           
        with open(os.path.join(self.save_path, self.FILENAME_GROUP), 'r') as f_group:
            self.groups = pickle.load(f_group)
        for group_name in self.groups:
            group = self.groups[group_name]
            member_list = [member.name for member in group.members]
            group.members = set([self.people[person_name] for person_name in member_list])
            
        for person_name in self.people:
            person = self.people[person_name]
            group_list = [group.name for group in person.groups]
            person.groups = set([self.groups[name] for name in group_list])

        self.save_path = load_path
   
