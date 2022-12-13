import sys
import os
from pathlib import Path
import argparse
from matplotlib import pyplot as plt


def main():

    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", required=True)
    parser.add_argument("-f", "--format", default = "cost")
    args = parser.parse_args()

    inFile = Path(args.input)

    if args.format == "cost":

        with inFile.open('r') as fin:
            data_list = [float(val) for val in fin.readlines()]
        plt.plot(data_list)
        plt.title("Cost vs Iteration")
        
    else:
        x = []
        y = []
        with inFile.open('r') as fin:
            for line in fin:
                x.append(float(line.split(": ")[0]))
                y.append(float(line.split(": ")[1]))
        plt.plot(x, y)
        plt.xscale('log')
        plt.title('Accepted Moves per Temp Step')
    
   
    plt.ion()
    plt.show(block= True)


if __name__ == "__main__":
    main()