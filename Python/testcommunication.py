import os
import numpy as np
from communication import *

def main():
    path = os.path.join(os.path.dirname(__file__), '../Data/public_field/public_field_sample_01.csv')
    print(path)
    b = Board(path)

    data = b.getData()
    score = b.getScore()
    print(data)
    print(score)
    agent_count = b.getAgentCount()

    while b.isEnded() == False:
        b.addAct(0, np.zeros(agent_count))
        b.addAct(1, np.ones(agent_count))
        print(b.getScore())

if __name__ == '__main__':
    main()