import numpy as np

LR = 0.01
N = 100

def tanh(x):
    return np.tanh(x)

def dtanh(x):
    return 1/np.cosh(x)**2

def loss(y, x):
    d = x - y
    return np.dot(d,d)

def dloss(x, y):
    d = (y - x)
    return d

class Network:
    def __init__(self):
        self.w = np.random.random((2,2))

    def forward(self, x):
        return np.tanh(np.dot(self.w, x))
    
    @staticmethod
    def getDy(yp, y):
        d = dloss(yp, y)
        return  d
    
    def backprop(self, x, y):
        l1 = np.dot(self.w, x)

        yp = tanh(l1)

        dy = self.getDy(yp, y)
        da = dy/dtanh(l1)
        
        dw = np.dot(da.reshape(2,1), x.reshape(1,2))

        self.w = self.w + (dw * LR)
        return


net = Network()

x = np.array([1,0])
y = np.array([0,1])

print("befor training:")
print(net.forward(x), loss(net.forward(x), y))
for i in range(N):
    net.backprop(x,y)

print("\nAfter training: ")
print(net.forward(x), loss(net.forward(x), y))
