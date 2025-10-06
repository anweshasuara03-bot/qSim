# 🏦 Bank Queue Simulation (Event-Driven)

## 📘 Introduction
This project simulates customers arriving at a bank and being served by tellers using an **event-driven simulation** approach.

The program compares two queuing models:

1. **Single queue** shared by all tellers.  
2. **Separate queue** for each teller.

The goal is to measure:
- Waiting times  
- Service times  
- Teller idle times  
- Overall system efficiency  

and compare the performance of both models.

---

## ⚙️ Compilation

To build the project, run:

```bash
make
```

## Running the program

### Usage
./bin/qSim #customers #tellers simulationTime avgService
 
### Example
./bin/qSim 100 4 60 2.3
