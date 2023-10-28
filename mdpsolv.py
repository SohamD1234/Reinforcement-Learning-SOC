import numpy as np

# MDP text file
fileName = "mdp-2-2.txt"

class DP_algo:
    def __init__(self, fileName):
        P = {}
        with open('../MDPS/' + fileName, "r") as file:
            for line in file:
                fields = line.strip().split()
                if fields[0] == "states":
                    n_S = int(fields[1])
                elif fields[0] == "actions":
                    n_A = int(fields[1])
                elif fields[0] == "gamma":
                    self.gamma = np.float128(fields[1])

            for i in range(n_S):
                P[i] = {}
                for j in range(n_A):
                    P[i][j] = []

            file.seek(0)  # Rewind to the beginning of the file
            for line in file:
                fields = line.strip().split()
                if fields[0] == "tran":
                    a = (np.float128(fields[5]), int(fields[3]), np.float128(fields[4]))
                    P[int(fields[1])][int(fields[2])].append(a)

            self.P = P

    def policy_iteration(self, theta=1e-10):
        rand_actions = np.random.choice(tuple(self.P[0].keys()), len(self.P))
        pi = lambda s: {s: a for s, a in enumerate(rand_actions)}[s]
        V = np.zeros(len(self.P),dtype=np.float128)

        while True:
            old_pi = {s: pi(s) for s in range(len(self.P))}

            while True:
                prev_V = V.copy()
                for s in range(len(self.P)):
                    Vs = []
                    for prob, next_state, reward in self.P[s][pi(s)]:
                        Vs.append(prob * (reward + self.gamma * prev_V[next_state]))
                    V[s] = np.float128(np.nansum(Vs))
                if np.max(np.abs(prev_V - V)) < theta:
                    break

            Q = np.zeros((len(self.P), len(self.P[0])), dtype=np.float128)

            for s in range(len(self.P)):
                for a in range(len(self.P[s])):
                    for prob, next_state, reward in self.P[s][a]:
                        Q[s][a] += np.float128(prob * (reward + self.gamma * V[next_state]))

            pi = lambda s: {s: a for s, a in enumerate(np.argmax(Q, axis=1))}[s]

            if old_pi == {s: pi(s) for s in range(len(self.P))}:
                break

        ac = [pi(i) for i in range(len(self.P))]
        return V,ac

    def value_iteration(self , theta = 1e-10):
        V = np.zeros(len(self.P) , dtype = np.float128)

        while True:
            Q=np.zeros((len(self.P) , len(self.P[0])) , dtype=np.float128)
            for s in range(len(self.P)):
                for a in range(len(self.P[s])):
                    for prob, next_state, reward in self.P[s][a]:
                        Q[s][a] += np.float128(prob * (reward + self.gamma * V[next_state]))
                        
            if np.max(np.abs(V - np.max(Q, axis=1))) < theta:
                break
            V = np.max(Q,axis=1)

            pi = lambda s : {s:a for s,a in enumerate(np.argmax(Q, axis=1))}[s]
            
        ac = [pi(i) for i in range(len(self.P))]
        return V,ac


algo = DP_algo(fileName)
V,pi = algo.value_iteration()
print(V)
print(pi)
with open("mdp2.txt" , 'w') as file:
    for i in range(len(algo.P)):
        file.write(f"{round(V[i], 7)}  {pi[i]}\n")